#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proc-common.h"

#define SLEEP_PROC_SEC  20
#define SLEEP_TREE_SEC  3

/*
 * Create this process tree:
 * A-+-B---D
 *   `-C
 */
void fork_procs(void)
{
        /*
         * initial process is A.
         */
        pid_t p1, p2, p3;
        int s1, s2, s3;

        change_pname("A");
        printf("A is created\n");
        printf("A: Created B, waiting for it to terminate...\n");

        p1 = fork();
        if (p1 < 0) {
                perror("A: fork");
                exit(1);
        }
        if (p1 == 0) {
                /* B */
                change_pname("B");
                printf("B: Created D, waiting for it to terminate...\n");
                p3 = fork();
                if (p3 < 0) {
                        perror("B: fork");
                        exit(1);
                }
                if (p3 == 0) {
                        /* D */
                        change_pname("D");
                        printf("D: Sleeping...\n");
                        sleep(SLEEP_PROC_SEC);
                        printf("D: Exiting...\n");
                        exit(13);
                }
                p3 = wait(&s3);
                explain_wait_status(p3, s3);
                printf("B: Exiting...\n");
                exit(19);
                }

        printf("A: Created C, waiting for it to terminate...\n");
        p2 = fork();


        if (p2 < 0) {
                perror("C: fork");
                exit(1);
        }
        if (p2 == 0) {
                /* C */
                change_pname("C");
                printf("C: Sleeping...\n");
                sleep(SLEEP_PROC_SEC);
                printf("C: Exiting...\n");
                exit(17);
        }

        p1 = wait(&s1);
        explain_wait_status(p1, s1);

        p2 = wait(&s2);
        explain_wait_status(p2, s2);


        printf("A: Exiting...\n");
        exit(16);
}

/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
*/
int main(void)
{
        pid_t pid;
        int status;

        /* Fork root of process tree */
        pid = fork();
        if (pid < 0) {
                perror("main: fork");
                exit(1);
        }
        if (pid == 0) {
                /* Child */
                fork_procs();
                exit(1);
        }

        //Father
        sleep(SLEEP_TREE_SEC);

        /* Print the process tree root at pid */
        show_pstree(pid);
        /* Wait for the root of the process tree to terminate */
        pid = wait(&status);
        explain_wait_status(pid, status);

        return 0;
}
