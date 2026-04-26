#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define TIME_QUANTUM 3  // Time slice for Round Robin

// Simulation table
typedef struct {
    int id;
    int arriveAfterTick;
    int maxTicks;
} SimEntry;

SimEntry simulation[] = {
    {0, 0, 10},
    {1, 2, 5},
    {2, 4, 8},
    {3, 6, 3},
    {4, 8, 6}
};

int simSize = 5;

// =====================
// Helper Functions
// =====================

PCB *createProcess(int pid, int arrivalTime, int burstTime) {
    PCB *p = malloc(sizeof(PCB));
    p->pid            = pid;
    p->arrivalTime    = arrivalTime;
    p->burstTime      = burstTime;
    p->remainingTime  = burstTime;
    p->priority       = 0;
    p->waitingTime    = 0;
    p->turnaroundTime = 0;
    p->programCounter = 0;
    p->state          = READY;
    return p;
}

void contextSwitch(PCB *current, PCB *next) {
    if (current != NULL) {
        printf("  [Context Switch] Saving PID %d | PC=%d\n",
               current->pid, current->programCounter);
        current->state = READY;
    }
    if (next != NULL) {
        printf("  [Context Switch] Loading PID %d | PC=%d\n",
               next->pid, next->programCounter);
        next->state = RUNNING;
    }
}

void printMetrics(PCB *completed[], int count) {
    printf("\n========== METRICS ==========\n");
    printf("%-5s %-10s %-10s %-12s %-13s\n",
           "PID", "Arrival", "Burst", "Waiting", "Turnaround");
    float avgWait = 0, avgTurnaround = 0;
    for (int i = 0; i < count; i++) {
        printf("%-5d %-10d %-10d %-12d %-13d\n",
               completed[i]->pid,
               completed[i]->arrivalTime,
               completed[i]->burstTime,
               completed[i]->waitingTime,
               completed[i]->turnaroundTime);
        avgWait       += completed[i]->waitingTime;
        avgTurnaround += completed[i]->turnaroundTime;
    }
    printf("\nAverage Waiting Time:    %.2f\n", avgWait / count);
    printf("Average Turnaround Time: %.2f\n", avgTurnaround / count);
    printf("==============================\n");
}

// =====================
// FCFS
// =====================

void runFCFS() {
    printf("\n====== FCFS SCHEDULING ======\n");

    Queue *readyQueue     = createQueue();
    PCB   *runningProcess = NULL;
    PCB   *completed[100];
    int    completedCount = 0;
    int    tick           = 0;

    while (completedCount < simSize) {

        printf("\n-- Tick %d --\n", tick);

        // Check for arriving processes
        for (int i = 0; i < simSize; i++) {
            if (simulation[i].arriveAfterTick == tick) {
                PCB *p = createProcess(simulation[i].id,
                                       simulation[i].arriveAfterTick,
                                       simulation[i].maxTicks);
                appendToQueue(readyQueue, p);
                printf("  PID %d arrived and added to queue\n", p->pid);
            }
        }

        // If CPU is free, dispatch next process from front of queue
        if (runningProcess == NULL && !isQueueEmpty(readyQueue)) {
            PCB *next = takeFromQueue(readyQueue, 0);
            contextSwitch(runningProcess, next);
            runningProcess = next;
            printf("  Dispatching PID %d\n", runningProcess->pid);
        }

        // Run current process for one tick
        if (runningProcess != NULL) {
            runningProcess->remainingTime--;
            runningProcess->programCounter++;
            printf("  PID %d running | Remaining: %d ticks\n",
                   runningProcess->pid, runningProcess->remainingTime);

            // Update waiting time for queued processes
            for (int i = 0; i < readyQueue->size; i++) {
                readyQueue->processes[i]->waitingTime++;
            }

            // Check if finished
            if (runningProcess->remainingTime == 0) {
                runningProcess->state         = FINISHED;
                runningProcess->turnaroundTime = tick + 1 - runningProcess->arrivalTime;
                printf("  PID %d FINISHED | Turnaround: %d | Waiting: %d\n",
                       runningProcess->pid,
                       runningProcess->turnaroundTime,
                       runningProcess->waitingTime);
                completed[completedCount++] = runningProcess;
                runningProcess = NULL;
            }
        }

        printQueue(readyQueue);
        tick++;
    }

    printMetrics(completed, completedCount);
}

// =====================
// Round Robin
// =====================

void runRoundRobin() {
    printf("\n====== ROUND ROBIN SCHEDULING (Quantum = %d) ======\n", TIME_QUANTUM);

    Queue *readyQueue     = createQueue();
    PCB   *runningProcess = NULL;
    PCB   *completed[100];
    int    completedCount = 0;
    int    tick           = 0;
    int    timeInCPU      = 0;  // tracks how long current process has been running

    while (completedCount < simSize) {

        printf("\n-- Tick %d --\n", tick);

        // Check for arriving processes
        for (int i = 0; i < simSize; i++) {
            if (simulation[i].arriveAfterTick == tick) {
                PCB *p = createProcess(simulation[i].id,
                                       simulation[i].arriveAfterTick,
                                       simulation[i].maxTicks);
                appendToQueue(readyQueue, p);
                printf("  PID %d arrived and added to queue\n", p->pid);
            }
        }

        // If no process is running, dispatch next from queue
        if (runningProcess == NULL && !isQueueEmpty(readyQueue)) {
            PCB *next = takeFromQueue(readyQueue, 0);
            contextSwitch(runningProcess, next);
            runningProcess = next;
            timeInCPU      = 0;
            printf("  Dispatching PID %d\n", runningProcess->pid);
        }

        // Run current process for one tick
        if (runningProcess != NULL) {
            runningProcess->remainingTime--;
            runningProcess->programCounter++;
            timeInCPU++;
            printf("  PID %d running | Remaining: %d | Time in CPU: %d/%d\n",
                   runningProcess->pid, runningProcess->remainingTime,
                   timeInCPU, TIME_QUANTUM);

            // Update waiting time for queued processes
            for (int i = 0; i < readyQueue->size; i++) {
                readyQueue->processes[i]->waitingTime++;
            }

            // Check if finished
            if (runningProcess->remainingTime == 0) {
                runningProcess->state         = FINISHED;
                runningProcess->turnaroundTime = tick + 1 - runningProcess->arrivalTime;
                printf("  PID %d FINISHED | Turnaround: %d | Waiting: %d\n",
                       runningProcess->pid,
                       runningProcess->turnaroundTime,
                       runningProcess->waitingTime);
                completed[completedCount++] = runningProcess;
                runningProcess = NULL;
                timeInCPU      = 0;

            // Check if time quantum is exceeded — preempt
            } else if (timeInCPU == TIME_QUANTUM) {
                printf("  PID %d time quantum expired — preempting\n",
                       runningProcess->pid);
                contextSwitch(runningProcess, NULL);
                appendToQueue(readyQueue, runningProcess);
                runningProcess = NULL;
                timeInCPU      = 0;
            }
        }

        printQueue(readyQueue);
        tick++;
    }

    printMetrics(completed, completedCount);
}

// =====================
// Main
// =====================

int main() {
    runFCFS();
    runRoundRobin();
    return 0;
}
