#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *createQueue() {
    Queue *queue = malloc(sizeof(Queue));
    queue->front = 0;
    queue->rear  = 0;
    queue->size  = 0;
    return queue;
}

void appendToQueue(Queue *queue, PCB *process) {
    if (queue->size == MAX_PROCESSES) {
        printf("Queue is full!\n");
        return;
    }
    queue->processes[queue->rear] = process;
    queue->rear++;
    queue->size++;
}

PCB *takeFromQueue(Queue *queue, int index) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }
    PCB *process = queue->processes[index];
    for (int i = index; i < queue->size - 1; i++) {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->rear--;
    queue->size--;
    return process;
}

int isQueueEmpty(Queue *queue) {
    return queue->size == 0;
}

void printQueue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        printf("  Ready Queue: [empty]\n");
        return;
    }
    printf("  Ready Queue: ");
    for (int i = 0; i < queue->size; i++) {
        printf("[PID %d | Remaining: %d] ", 
               queue->processes[i]->pid,
               queue->processes[i]->remainingTime);
    }
    printf("\n");
}
