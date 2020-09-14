#ifndef QUEUE_H_
#define QUEUE_H_

#include <unistd.h>

//Node struct
struct Node_t {
        int id;
        pid_t pid;
        char* name;
        int priority;
        struct Node_t* next;
};

typedef struct Node_t Node;

//useful functions for queue manipulation
void queue_insert(int id, pid_t pid, char* name, int priority);
void queue_remove(pid_t pid);

#endif
