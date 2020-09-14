#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "proc-common.h"
#include "queue.h"

// Compile-time parameters.
#define SCHED_TQ_SEC 5                // time quantum
#define TASK_NAME_SZ 60               // maximum size for a task's name

Node* head;
Node* tail;
int queue_items;

// SIGALRM handler
static void
sigalrm_handler(int signum)
{
        if (kill(head->pid, SIGSTOP) < 0) {
                perror("Error in stopping process\n");
                exit(1);
        }
}

// SIGCHLD handler
static void
sigchld_handler(int signum)
{
        int p, status;

        for (;;) {
                //Wait for any child to change its state
                p = waitpid(-1, &status, WUNTRACED | WNOHANG);
                //No child has changed state
                if (p == 0) break;
                explain_wait_status(p, status);
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                        //in here the child has been terminated either normally or by a signal
                        //remove it from the queue and update it
                        queue_remove(head->pid);
                        head = head->next;
                        tail = tail->next;

                        if (kill(head->pid, SIGCONT) < 0) {
                                perror("Error\n");
                                exit(1);
                        }
                        alarm(SCHED_TQ_SEC);
                }
                if (WIFSTOPPED(status)) {
                        //in here the child has been stopped by a signal
                        head = head->next;
                        tail = tail->next;

                        if (kill(head->pid, SIGCONT) < 0) {
                                perror("Error\n");
                                exit(1);
                        }
                        alarm(SCHED_TQ_SEC);
                }
        }

}

//Install two signal handlers.
// One for SIGCHLD, one for SIGALRM.
// Make sure both signals are masked when one of them is running.
static void
install_signal_handlers(void)
{
        sigset_t sigset;
        struct sigaction sa;

        sa.sa_handler = sigchld_handler;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGCHLD);
        sigaddset(&sigset, SIGALRM);
        sa.sa_mask = sigset;
        if (sigaction(SIGCHLD, &sa, NULL) < 0) {
                perror("sigaction: sigchld");
                exit(1);
        }

        sa.sa_handler = sigalrm_handler;
        if (sigaction(SIGALRM, &sa, NULL) < 0) {
                perror("sigaction: sigalrm");
                exit(1);
        }

        // Ignore SIGPIPE, so that write()s to pipes
        // with no reader do not result in us being killed,
        // and write() returns EPIPE instead.
        if (signal(SIGPIPE, SIG_IGN) < 0) {
                perror("signal: sigpipe");
                exit(1);
        }
}

int main(int argc, char *argv[])
{
        int nproc, i;
        pid_t pid;
        queue_items = 0;

        nproc = argc; // number of proccesses goes here
        head = (Node*) malloc(sizeof(Node));
        head->next = NULL;

        if (nproc - 1 == 0) {
               fprintf(stderr, "Scheduler: No tasks. Exiting...\n");
               exit(1);
        }

        // For each of argv[1] to argv[argc - 1],
        // create a new child process, add it to the process list.
        for (i = 1; i < nproc; i++) {
                pid = fork();
                if (pid != 0) {
                        queue_insert(i, pid, argv[i]);
                }
                else {
                        char executable[] = "prog";
                        char *newargv[] = {executable, NULL};
                        char *newenviron[] = {NULL};

                        raise(SIGSTOP);
                        execve(executable, newargv, newenviron);
                }
        }
        //Queue is circular
        head = head->next;
        free(tail->next);
        tail->next = head;

        // Wait for all children to raise SIGSTOP before exec()ing.
        wait_for_ready_children(nproc - 1);

        // Install SIGALRM and SIGCHLD handlers.
        install_signal_handlers();

        //Start first child
        if (kill(head->pid, SIGCONT) < 0) {
                perror("Error in starting first child\n");
                exit(1);
        }
        //alarm in tq secs
        alarm(SCHED_TQ_SEC);

        // loop forever  until we exit from inside a signal handler.
        while (pause())
                ;

        // Unreachable
        fprintf(stderr, "Internal error: Reached unreachable point\n");
        return 1;
}
