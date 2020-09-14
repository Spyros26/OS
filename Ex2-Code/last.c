#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <sys/wait.h>

#include "proc-common.h"
#include "tree.h"



void compute_tree(struct tree_node *n, int fd) {
        int  i, s, res, *number, temp;
        pid_t p;


        printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), n->name);

        change_pname(n->name);
        int pfd[2*n->nr_children];

        for (i = 0; i < n->nr_children; i++ ) {
                printf("%s creates child %s.\n",n->name, (n->children+i)->name);

                //Open a pipe for every child
                if (pipe(pfd + 2*i) < 0) {
                        perror("Error at creating pipe\n");
                        exit(1);
                }

                p = fork();
                if (p < 0) {
                        perror("fork");
                        exit(1);
                }

                if (p == 0) {
                        change_pname((n->children+i)->name);
                        if ((n->children+i)->nr_children == 0) {
                                printf("I'm number %s\n", (n->children+i)->name);
                                sleep(1); //sleep so that the tree can be shown
                                int num = atoi((n->children+i)->name);
                                close(pfd[2*i]);
                                // Write the number into the pipe
                                if (write(pfd[2*i+1], &num, sizeof(num)) != sizeof(num)) {
                                perror("child: write to pipe");
                                }

                                exit(0);
                        }
                        else compute_tree(n->children + i, pfd[2*i + 1]);
                }
        }

        //number is the value of every leaf
        number = (int*) malloc (n->nr_children * sizeof(int));

        for(i = 0; i < n->nr_children; i++) {
                if (read(pfd[2*i], &temp, sizeof(temp)) != sizeof(temp)) {
                        perror("Error at reading from pipe\n");
                        exit(1);
                }
        *(number + i) = temp;
        }

        //Calculate
        switch(strcmp(n->name,"+")) {
                case 0:
                        res = *(number + 1) + *number;
                        break;
                default:
                        res = *(number + 1) * *number;
        }


        //Pass the result to father
        if (write(fd, &res, sizeof(res)) != sizeof(res)) {
                perror("Error at writing to pipe\n");
                exit(1);
        }

        //Wait for the children to die
        for (i = 0; i < n->nr_children; i++) {
                p = wait(&s);
                explain_wait_status(p,s);
        }


        exit(0);
}



int main(int argc, char *argv[])
{
        struct tree_node *root;
        int status, pfd[2], res;
        pid_t pid;

        if (argc != 2) {
                fprintf(stderr, "Usage: %s <input_tree_file>\n", argv[0]);
                exit(1);
        }

        if (pipe(pfd) < 0) {
                perror("Error at creating pipe\n");
                exit(1);
        }

        root = get_tree_from_file(argv[1]);

        pid = fork();
        if (pid < 0) {
                perror("Error at fork\n");
                exit(1);
        }
        if (pid == 0) {
                close(pfd[0]);
                compute_tree(root, pfd[1]);
                exit(0);
        }
        sleep(0.1);
        show_pstree(pid);

        close(pfd[1]);
        if (read(pfd[0], &res, sizeof(res)) != sizeof(res)) {
                perror("Error at reading from pipe\n");
                exit(1);
        }

        pid = wait(&status);
        explain_wait_status(pid, status);

        printf("Outcome is %d\n", res);

        return 0;
}