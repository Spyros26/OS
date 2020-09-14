#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "tree.h"
#include "proc-common.h"

#define TIME_AMOUNT 5

void make_tree(struct tree_node *n) {
int i,status;
pid_t pid;

printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), n->name);

        if (n->nr_children != 0) {
                for (i = 0; i < n->nr_children; i++) {
                        printf("%s creates child %s and waits for it to terminate...\n",n->name, n->children[i].name);

                        pid = fork();
                        if (pid < 0) {
                                perror("fork");
                                exit(1);
                        }
                        if (pid == 0) {
                                make_tree(&n->children[i]);
                        }
                }
        }
        else {
                printf("%s has no children, so it sleeps...\n", n->name);
        }
        sleep(TIME_AMOUNT);
        printf("%s exiting...\n", n->name);
        //sleep(1);
        for (i = 0; i < n->nr_children; i++) {
                pid = wait(&status);
                explain_wait_status(pid,status);
        }

        exit(0);
}

int main(int argc, char *argv[])
{
        struct tree_node *root;

        if (argc != 2) {
                fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
                exit(1);
        }

        root = get_tree_from_file(argv[1]);
        print_tree(root);
        make_tree(root);

        return 0;
}
