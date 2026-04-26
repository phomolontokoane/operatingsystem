#ifndef PROCESS_H
#define PROCESS_H

#define READY    0
#define RUNNING  1
#define FINISHED 2

typedef struct {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int programCounter;
    int state;
} PCB;

#endif
