#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tree.h"
#include "proc-common.h"


void make_tree_dfs(struct tree_node *node)
{
        change_pname(node->name);

        int i;
        int status;
        pid_t p[1024];

        // Create children
        if (node->nr_children != 0) {
                for (i = 0; i < node->nr_children; i++) {
                        p[i] = fork();
                        if (p[i] < 0) {
                                perror("fork");
                                exit(1);
                        }
                        if (p[i] == 0) {
                                make_tree_dfs(&node->children[i]);
                        }
                }
        }

        wait_for_ready_children(node->nr_children);

        //Suspend self
        raise(SIGSTOP);

        printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), node->name);

        for (i = 0; i < node->nr_children; i++) {
                kill (p[i],SIGCONT);
                p[i] = wait(&status);
                explain_wait_status(p[i],status);
        }

        exit(0);
}

/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 */
int main(int argc, char *argv[])
{
        pid_t pid;
        int status;
        struct tree_node *root;

        if (argc != 2){
                fprintf(stderr, "Usage: %s <tree_file>\n", argv[0]);
                exit(1);
        }

        /* Read tree into memory */
        root = get_tree_from_file(argv[1]);

        /* Fork root of process tree */
        pid = fork();
        if (pid < 0) {
                perror("main: fork");
               exit(1);
        }
        if (pid == 0) {
                /* Child */
                make_tree_dfs(root);
                exit(1);
        }

        // Father
        wait_for_ready_children(1);

        /* Print the process tree root at pid */
        show_pstree(pid);

        kill(pid, SIGCONT);
       /* Wait for the root of the process tree to terminate */
        pid = wait(&status);
        explain_wait_status(pid, status);

        return 0;
}
