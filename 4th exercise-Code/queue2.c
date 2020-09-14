#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue2.h"

Node* head;
Node* tail;
int queue_items;

void queue_insert(int id, pid_t pid, char* name, int priority) {
        Node* new = (Node*) malloc(sizeof(Node));
        new->id = id;
        new->pid = pid;
        new->name = name;
        new->priority = priority;
        Node* temp = head;
        while(temp->next != NULL)
                temp = temp->next;
        temp->next = new;
        tail = new;
        queue_items++;
}

void queue_remove(pid_t pid) {
        Node* temp = head;
        while(temp->next->pid != pid)
                temp = temp->next;
        free(temp->next);
        temp->next = temp->next->next;
        queue_items--;
        if (queue_items == 0) {
                printf("No processes left.\n");
                exit(1);
        }
}
