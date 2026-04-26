#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

#define MAX_PROCESSES 100

typedef struct {
    PCB *processes[MAX_PROCESSES];
    int front;
    int rear;
    int size;
} Queue;

Queue *createQueue();
void appendToQueue(Queue *queue, PCB *process);
PCB *takeFromQueue(Queue *queue, int index);
int isQueueEmpty(Queue *queue);
void printQueue(Queue *queue);

#endif
