/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for process struct, please refer to .c file for
 *          documentation of each function.
 */

#include "../include/helper.h"
#include "../include/process.h"
#include "../include/details.h"

// Prints at the start of execution
void print_execution(long long int *time, Process *process){
    print_running(time, process);
}

// Prints the process running
void print_running(long long int *time, Process *process){
    if(!(process->started)){
        process->started = 1;
    }
    printf("%lli, RUNNING, id=%lli, remaining-time=%lli", *time, 
                            process->process_id, process->rem_time);
}

// Prints a finished process
void print_finished(long long *time, Process *process, int num_proc_left){
    process->end_time = *time;
    printf("%lli, FINISHED, id=%lli, proc-remaining=%d\n", *time,
                                 process->process_id, num_proc_left);
}

// Prints the statistics of a process
void print_stats(Process_list *processes, long long int time){
    calculate_throughput(processes);
    calculate_overhead(processes);
    printf("Makespan %lld\n", time);
}

// Calculates throughput for final statistics
void calculate_throughput(Process_list *processes){
    Int_list *throughput_values = malloc(sizeof(Int_list));
    int avg_throughput = 0;

    int total_intervals = 1;
    int lower_bound = 1;
    int upper_bound = 60;
    int current_total = 0;
    
    Node *current = processes->head;
    int i = 0;
    while(current != NULL){
        long long int end_time = current->process->end_time;
        if(end_time <= upper_bound && end_time >= lower_bound){
            current_total++;
            current = current->next;
            avg_throughput++;
        } else {
            total_intervals++;
            insert(throughput_values, current_total);
            lower_bound += 60;
            upper_bound += 60;
            current_total = 0;
            i++;
        }   
    }
    insert(throughput_values, current_total);
    int max = 0, min = 9999;
    Int_node *current_int = throughput_values->head;
    while(current_int !=NULL){
        int curr_int = current_int->data;
        if(max < curr_int){
            max = curr_int;
        }
        if(min > curr_int){
            min = curr_int;
        }
        current_int = current_int->next;
    }
    avg_throughput = ceil((long double)avg_throughput/total_intervals);
    printf("Throughput %d, %d, %d\n", avg_throughput, min, max);
}

// Calculates overhead for final statistics
void calculate_overhead(Process_list *processes){
    Node *current;
    int size = processes->size;
    long double max = 0;
    long double total_overhead = 0;
    long long int total_turnaround = 0;


    current = processes->head;
    while(current != NULL){
        long double t_time = current->process->end_time - 
                                         current->process->time_arrived;
        long double overhead = t_time/current->process->job_time;
        if(max < overhead){
            max = overhead;
        }
        total_turnaround += t_time;
        total_overhead += overhead;
        current = current->next;
    }
    total_overhead = (total_overhead / size);
    total_turnaround = ((total_turnaround) / size) + 
                                (total_turnaround % size != 0);;

    printf("Turnaround time %lli\n", total_turnaround);
    printf("Time overhead %.2Lf %.2Lf\n", max, total_overhead);
}

// Prints the memory allocated to process, if any
void print_allocation(Process *process, struct Details *details,
                                                 Mem_block *memory){
    if(strcmp(details->m_algo, UNLIMITED)){
        int pages = (details->memsize)/PAGE_SIZE;
        int mem_used=0;
        for(int i = 0; i < pages; i++){
            if(memory[i].process_id != 0){
                mem_used++;
            }
        }
        long double memusage = ceil(100*((long double)mem_used/pages));
        printf(", load-time=%lli, mem-usage=%.0Lf%%, mem-addresses=", 
                                            process->load_time, memusage);
        print_occupied(process,memory,pages);
    }
    printf("\n");
}

// Prints memory block indexes for a given process 
void print_occupied(Process *process, Mem_block *memory_arr, int n_pages){
    Int_list *used_index = malloc(sizeof(Int_list));
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].process_id == process->process_id){
            insert(used_index, i);
        }
    }
    print_list(used_index);
}

// Finds the index of a process in memory
int find_id_index(Mem_block *memory_arr, int size, long long int id){
    int index = 0;
    for(int i = 0; i < size; i++){
        if(memory_arr[i].process_id == id){
            index = i;
        }
    }

    return index;
}

// FInds the first instance of a process in memory
int find_id_instance(Mem_block *memory_arr, int size, long long int id){
    for(int i = 0; i < size; i++){
        if(memory_arr[i].process_id == id){
            return i;
        }
    }

    return 0;
}

// Counts the number of allocated memory blocks for a given process
int id_alloted_count(Mem_block *memory_arr, int size, long long int id){
    int count = 0;
    
    for(int i = 0; i < size; i++){
        if(memory_arr[i].process_id == id){
            count++;
        }
    }

    return count;
}

// returns the minimum of two ints
int get_min(int a, int b){
    if(a < b){
        return a;
    }
    return b;
}