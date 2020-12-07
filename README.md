# process_scheduler_C
Process scheduler and memory manager simulation written using C standard library. An implementation of a process scheduler using essential scheduling & memory management algorithms.

Received 80% for the project (90% code quality).


## Documentation
- [Specification](https://github.com/garviin/process_scheduler_C/blob/master/project_specs.pdf)(Written by University of Melbourne)

## Files written by Garvin
#### Scheduler Main File: [scheduler.c](https://github.com/garviin/process_scheduler_C/blob/master/src/scheduler.c)
- Main file that contains essential process scheduling and memory management algorithms.
- Simulates the following process scheduling algorithms:
    - First Come First Serve (FCFS)
    - Round Robin
- Simulates the following memory management algorithms:
    - Swapping (with paging)
    - Virtual Memory
#### Argument Parser: [details.c](https://github.com/garviin/process_scheduler_C/blob/master/src/details.c)
- To parse arguments from the command line.
#### Linked List Implementation: [linkedlist.c](https://github.com/garviin/process_scheduler_C/blob/master/src/linkedlist.c)
- Implementation of linked list, with each node as a struct that represents a process.
- Implements standard methods and attributes of a linkedlist.
#### Processes Struct: [process.c](https://github.com/garviin/process_scheduler_C/blob/master/src/process.c)
- A struct that represents a process in the simulation.
#### Helper Functions: [helper.c](https://github.com/garviin/process_scheduler_C/blob/master/src/helper.c)
- Functions & algorithms to calculate statistics and prints results of the simulation.

