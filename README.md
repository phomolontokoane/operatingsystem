# Operating System Scheduler Simulator

This repository contains a simple CPU scheduling simulator written in C. It demonstrates two scheduling algorithms using a basic process queue:

- First-Come, First-Served (FCFS)
- Round Robin (RR)

The simulator uses a fixed set of processes and prints step-by-step scheduling details, including context switches, queue state, process execution, and final metrics.

## Repository Structure

- `dispatcher.c` - Main simulation logic, process creation, and scheduler implementations.
- `queue.c` / `queue.h` - Queue data structure used to manage ready processes.
- `process.h` - Process control block (`PCB`) definition and state constants.
- `makefile` - Build instructions for compiling the simulator.

## Build

From the project root, run:

```bash
make
```

This compiles `dispatcher.c` and `queue.c` into the executable `dispatcher`.

## Run

Run the simulator with:

```bash
./dispatcher
```

The program will execute both FCFS and Round Robin scheduling simulations sequentially and display detailed output.

## Scheduling Behavior

- `runFCFS()` simulates a simple first-come, first-served scheduler.
- `runRoundRobin()` simulates Round Robin scheduling with a time quantum of 3 ticks.

Each simulation tracks per-process waiting time and turnaround time, then prints average metrics at the end.

## Customization

Simulation entries are defined in `dispatcher.c` inside the `simulation[]` array. Each entry includes:

- `id` - process ID
- `arriveAfterTick` - arrival time in ticks
- `maxTicks` - total CPU burst length

To change the workload, modify `simulation[]` and update `simSize` if necessary.

## Clean

Remove the compiled executable with:

```bash
make clean
```

## Notes

This project is intended for learning and demonstration rather than production use. The queue implementation is simple and designed to support the scheduling simulation.
