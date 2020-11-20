/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for helper functions, please refer to .c file for
 *          documentation of each function.
 */

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/process.h"
#include "../include/linkedlist.h"
#include "../include/details.h"

// Contstants for memory related operations
#define PAGE_SIZE 4
#define VIRTUAL_MIN_PAGES 4
#define LOAD_TIME 2

// Struct path for a page in memory.
typedef struct Mem_block{
    long long int process_id;
    long long int last_executed;
} Mem_block;


void print_stats(Process_list *processes, long long int time);
void print_running(long long int *time, Process *process);
void print_finished(long long *time, Process *process, int num_proc_left);
void calculate_throughput(Process_list *processes);
void calculate_overhead(Process_list *processes);
void print_execution(long long int *time, Process *process);
void print_allocation(Process *process, struct Details *details, Mem_block *memory);
void print_occupied(Process *process, Mem_block *memory_arr, int n_pages);
int find_id_index(Mem_block *memory_arr, int size, long long int id);
int find_id_instance(Mem_block *memory_arr, int size, long long int id);
int id_alloted_count(Mem_block *memory_arr, int size, long long int id);
int get_min(int a, int b);

#endif