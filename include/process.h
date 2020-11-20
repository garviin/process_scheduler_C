/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for process struct, please refer to .c file for
 *          documentation of each function.
 */
#ifndef PROCESS_H
#define PROCESS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Struct path to store a process
typedef struct Process{
    long long int time_arrived, process_id, memory_size_req, job_time;
    long long int rem_time, end_time, load_time;
    int started;
} Process;

// Node of a process linkedlist
typedef struct Node {
  Process *process;
  struct Node *next;
  struct Node *prev;
} Node;

//linkedlist of processes
typedef struct Process_list{
    Node *head;
    Node *tail;
    int size;
} Process_list;

Process_list *read_processes(char *filename);
void insert_beginning(Process_list *list, Process *new_process);
void insert_end(Process_list *list, Process *new_process);
void insert_sorted(Process_list *list, Process *new_process);
void remove_head(Process_list *list);
void print_processes_head(Process_list *list);
Process *find_process(Process_list *list, long long int id);
void sort_processes(Process_list *list);

#endif