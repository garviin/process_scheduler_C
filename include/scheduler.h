#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/details.h"
#include "../include/process.h"
#include "../include/helper.h"
#include "../include/linkedlist.h"

#define SWAPPING "p"
#define VIRTUAL_MEMORY "v"

#define MAXIMUM_INT 100000000
#define MINIMUM_INT -10000000

struct Details *read_arguments(int argc, char** argv);

long long int *simulate_ff(Process_list *processes, Process_list *finished, struct Details *details, Mem_block *memory_arr);
long long int *simulate_rr(Process_list *processes, Process_list *finished, struct Details *details,Mem_block *memory_arr);

void refresh_queue(Process_list *processes, Process_list *queue, long long int *current_time);
int tick_time(Process_list *processes, Process_list *queue, long long int *current_time);

void simulate_ff_p(Process_list *processes, long long int mem_size);
int allocate_memory_p(Process_list *processes, Mem_block *memory_arr, int n_pages, long long int *time);
int allocate_memory(Mem_block *memory, Process_list *processes, struct Details *details, long long int *time);
void evict_memory(Mem_block *memory_arr, long long int size, Process *process, long long int current_time);
int available_memory(Mem_block *memory_arr, long long int n_pages, long long int req_memory);
Mem_block *init_memory(struct Details *details);
int allocate_memory_deep(Process *process, Mem_block *memory_arr, int req_pages, int available_block,long long int *time, int size);
void execute_processes(Process_list *processes, struct Details *details);
int allocate_memory_a(Mem_block *memory, Process_list *processes, struct Details *details, long long int *time);
int evict_least_recent(Mem_block *memory_arr, long long int n_pages);
int memory_allocated(Mem_block *memory_arr, long long int n_pages, Process *process);
void evict(Mem_block *memory_arr, long long int size, Process *process, long long int current_time);
void update_time_rr(long long int *time, Process *process, int quantum);
void update_time_ff(long long int *time, Process *process);
void update_time(struct Details *details, long long int *time, Process *process);
long long int *simulate_execution(Process_list *processes, Process_list *finished, struct Details *details, Mem_block *memory_arr);
void finish_execution(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished);
void finish_ff(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished);
void finish_rr(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished);
void next_process(Process_list *processes, Process_list *queue, long long int *current_time, Process *current_process);
void evict_memory_v(Mem_block *memory_arr, long long int size, long long int current_time, int req_pages, Process *process);
void evict_v(Mem_block *memory_arr, long long int size, Process *process, long long int current_time, int req_pages);
int available_memory_count(Mem_block *memory_arr, long long int n_pages);
int allocate_memory_v(Process_list *processes, Mem_block *memory_arr, int n_pages, long long int *time, struct Details *details);
int index_of_least_recent(Mem_block *memory_arr, long long int n_pages);
int available_memory_total(Mem_block *memory_arr, long long int n_pages);
int allocate_memory_deep_v(Process *process, Mem_block *memory_arr, int req_pages, int available_block,long long int *time, int size);
int memory_free_exist(Mem_block *memory_arr, int size);
void evict_memory_cm(Mem_block *memory_arr, long long int size, long long int current_time, int req_pages, Process *process);
int allocate_memory_cm(Process_list *processes, Mem_block *memory_arr, int n_pages, long long int *time);
int index_of_most_recent(Mem_block *memory_arr, long long int n_pages);

#endif