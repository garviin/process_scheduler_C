
#include "../include/scheduler.h"

int main(int argc, char** argv) {
    // Reading command line arguments
    struct Details *details = read_arguments(argc, argv);

    // Reading processes
    Process_list *process_list= read_processes(details->filename);

    // Execute all processes
    execute_processes(process_list, details);

    exit(EXIT_SUCCESS);
}

// Executes processes read from test cases
void execute_processes(Process_list *processes, struct Details *details){
    // List to keep track of finished process
    Process_list *finished = malloc(sizeof(Process_list));

    // Array of memory blocks to simulate memory space
    Mem_block *memory = init_memory(details);

    // Simulating execution
    long long int *end_time = 0;
    end_time = simulate_execution(processes, finished, details, memory);

    // Printing statistics
    print_stats(finished, *end_time);
}

// Refreshing queue of processes according to arrival time
void refresh_queue(Process_list *processes, Process_list *queue,
                   long long int *current_time){
    Node *current = processes->head;
    while(current != NULL){
        // Only push processes that has arrived
        if(current->process->time_arrived > *current_time){
            return;
        }
        // Pushing to the end of queue
        insert_end(queue, current->process);

        // Remove pushed process from the list of waiting processes
        remove_head(processes);
        current = processes->head;
    }
}

// Allocate memory according to input algorithm
int allocate_memory(Mem_block *memory, Process_list *processes,
                    struct Details *details, long long int *time){
    int load_time = 0;
    int pages = (details->memsize)/PAGE_SIZE;

    // Call allocate memory functions according to input arguments
    if(strcmp(details->m_algo, UNLIMITED)){ 
        // long long int *end_time = 0;
        if(!strcmp(details->m_algo, SWAPPING)){
            load_time = allocate_memory_p(processes, memory, pages, time);
        } else if(!strcmp(details->m_algo, VIRTUAL_MEMORY)){
            load_time = allocate_memory_v(processes, memory, pages, time,
                                                                 details);
        } else if(!strcmp(details->m_algo, BYO_M)){
            load_time = allocate_memory_v(processes, memory, pages, time,
                                                                 details);
        } 
    }
    // Returns load time of memory allocation
    return load_time;
}

// Counts the number of continuous memory blocks
int available_memory(Mem_block *memory_arr, long long int n_pages,
                     long long int req_memory){
    int count = 0;
    int index = 0;
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].process_id == 0){
            count++;
        } else{
            count = 0;
            index = i+1;
        }
    }
    if(count>=req_memory){
        return index;
    }
    return -1;
}

// Initializes memory array of simulation
Mem_block *init_memory(struct Details *details){
    // Ignore if memory management algorithm is not given
    if(!details->m_algo){
        return NULL;
    }

    // Total pages in the memory
    int size = (details->memsize)/PAGE_SIZE;

    // 0 indicates that memory block is empty
    Mem_block *memory_arr = (Mem_block*)malloc(size*sizeof(Mem_block));
    for(int i=0; i < size; i++){
        memory_arr[i].process_id = 0;
        memory_arr[i].last_executed = 0;
    }

    // Returns pointer of initialized memory array 
    return memory_arr;
}

// Allocation algorithm for swapping algorithm
int allocate_memory_p(Process_list *processes, Mem_block *memory_arr,
                      int n_pages, long long int *time){
    // Current process being executed
    Process *process = processes->head->process;
    // Pages required by the process
    int req_pages = (process->memory_size_req)/PAGE_SIZE;

    // Index of the first page of memory block to be given to process
    int available_block = available_memory(memory_arr,n_pages,req_pages);

    // Number of available pages
    int available_count = available_memory_total(memory_arr,n_pages);

    // Time spent loading the process into memory
    int load_time = 0;

    // Check if there is space available in memory, if not, evict least 
    // recently used memory block
    if(available_count >= req_pages){
        load_time = allocate_memory_deep(process, memory_arr, req_pages,
                                         available_block, time,n_pages);
        process->load_time = load_time;
    } else {
        // Keep evicting memory until required space is available
        while(available_memory_total(memory_arr,n_pages) < req_pages){
            long long int id_to_evict = evict_least_recent(memory_arr, 
                                                            n_pages);
            if(id_to_evict == process->process_id){
                break;
            }
            Process *to_evict = find_process(processes, id_to_evict);
            evict_memory(memory_arr, n_pages, to_evict, *time);
        }
        int available_block = available_memory(memory_arr,n_pages,
                                                            req_pages);
        load_time = allocate_memory_deep(process, memory_arr, req_pages,
                                         available_block, time, n_pages);
        process->load_time = load_time;
    }
    return load_time;
}

// Second layer function to allocate memory in swapping algorithm
int allocate_memory_deep(Process *process, Mem_block *memory_arr,
                         int req_pages, int available_block,
                         long long int *time, int size){
    // Keep track of used memory indexes
    Int_list *used_index = malloc(sizeof(Int_list));
    // Keep track of load time
    int load_time = 0;
    int count = 0;

    // Assumes if memory already exists within memory, no need to allocate
    if(find_id_index(memory_arr, size, process->process_id) != 0){
            return 0;
    }

    // Allocates the required n pages in memory array
    for(int i = 0; 
        count < req_pages || memory_free_exist(memory_arr, size); 
        i++){
        if(memory_arr[i].process_id == 0){
            memory_arr[i].process_id = process->process_id;
            memory_arr[i].last_executed = *time;
            insert(used_index, i);
            load_time++;
            count++;
        } else if (memory_arr[i].process_id == process->process_id){
            count++;
        }

        if(process->memory_size_req == count*PAGE_SIZE){
            break;
        }
    }

    // Returns total load time (2 seconds per page)
    return load_time*LOAD_TIME;
}

// Printing eviction of memory block for a given process
void evict_memory(Mem_block *memory_arr, long long int size, 
                  Process *process, long long int current_time){
    printf("%lli, EVICTED, mem-addresses=", current_time);
    print_occupied(process,memory_arr,size);
    printf("\n");
    evict(memory_arr, size, process, current_time);
}

// Function to evict memory block for the given process
void evict(Mem_block *memory_arr, long long int size, Process *process, 
           long long int current_time){
    for(int i = 0; i < size; i++){
        if(memory_arr[i].process_id == process->process_id){
            memory_arr[i].process_id = 0;
            memory_arr[i].last_executed = 0;
        }
    }
    process->load_time = 0;
}

// Check if the memory for a given function is already allocated
int memory_allocated(Mem_block *memory_arr, long long int n_pages,
                     Process *process){
    // If process id exists within memory, its already allocated
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].process_id == process->process_id){
            return 1;
        }
    }
    return 0;
}

// Returns the process_id for the least recently executed process
int evict_least_recent(Mem_block *memory_arr, long long int n_pages){
    long long int min_t = MAXIMUM_INT;
    long long int process_id = 0;
    // Find the least recently accessed memory block
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].last_executed < min_t && 
           memory_arr[i].process_id != 0){
            process_id = memory_arr[i].process_id;
            min_t = memory_arr[i].last_executed;
        }
    }
    return process_id;
}

// Simulates execution, and returns the end execution time
long long int *simulate_execution(Process_list *processes, 
                                  Process_list *finished, 
                                  struct Details *details, 
                                  Mem_block *memory_arr){
    // Queue for processes
    Process_list *queue = malloc(sizeof(Process_list));
    
    // Keep track of time
    long long int time = 0;
    long long int *current_time = &time;
    
    // Number of pages in memory
    int pages = (details->memsize)/PAGE_SIZE;
    
    // Loading queue with processes
    refresh_queue(processes, queue, current_time);
    Process *current_process = queue->head->process;

    // Go through each process
    while(queue->size != 0 || processes->size != 0){
        // Allocating memory
        int load_time = allocate_memory(memory_arr, queue, details,
                                                         current_time);
        // Printing simulation details
        print_execution(current_time, current_process);
        *current_time += load_time;
        print_allocation(current_process,details, memory_arr);

        // Updating time
        update_time(details, current_time, current_process);
        
        remove_head(queue);
        refresh_queue(processes, queue, current_time);

        // Finishing up
        finish_execution(details, memory_arr,pages,current_process,
                         current_time,queue,finished);
        
        // Move on to next process
        if(!strcmp(details->s_algo, BYO_S)){
            sort_processes(queue);
        }
        if(queue->size != 0){
            current_process = queue->head->process;
        } else if(processes->size != 0){
            while(queue->size == 0){
                *current_time += 1;
                refresh_queue(processes, queue, current_time);
            }
            current_process = queue->head->process;
        }
    }

    return current_time;
}

// Updates time according to given algorithm
void update_time(struct Details *details, long long int *time,
                 Process *process){
    if(!strcmp(details->s_algo, FIRST_SERVE) ||
       !strcmp(details->s_algo, BYO_S)){
        update_time_ff(time, process);
    } else if(!strcmp(details->s_algo, ROUND_ROBIN)){
        update_time_rr(time, process, details->quantum);
    } else{
        printf("Need scheduling algorithm!\n");
        exit(EXIT_FAILURE);
    }
}

// Updates time for first serve algorithm
void update_time_ff(long long int *time, Process *process){
    *time += process->rem_time;
}

// Updates time for round robin algorithm
void update_time_rr(long long int *time, Process *process, int quantum){
    // Update time by min(remaining_time, quantum)
    if(process->rem_time < quantum){
        *time += process->rem_time;
        process->rem_time = 0;
    } else{
        process->rem_time -= quantum;
        *time += quantum;
    }
}

// Finishes execution according to given algorithm
void finish_execution(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished){
    if(!strcmp(details->s_algo, FIRST_SERVE) ||
       !strcmp(details->s_algo, BYO_S)){
        finish_ff(details, memory_arr, pages, process, time, queue, 
                                                                finished);
    } else if(!strcmp(details->s_algo, ROUND_ROBIN)){
        finish_rr(details, memory_arr, pages, process, time, queue, 
                                                                finished);
    } else{
        printf("Need scheduling algorithm!\n");
        exit(EXIT_FAILURE);
    }
}

// Final stage of ff algorithm: evicts memory, and print finished process
void finish_ff(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished){

    // If memory is not unlimited, evict memory used
    if(strcmp(details->m_algo, UNLIMITED)){
        evict_memory(memory_arr, pages, process, *time);
    }
    // Print finished process
    print_finished(time, process, queue->size);

    // Insert into finished array to keep track of finished processes
    insert_end(finished, process);
}

// Final stage of round robin algorithm
void finish_rr(struct Details *details, Mem_block *memory_arr,
                      int pages, Process *process,long long int *time,
                      Process_list *queue, Process_list *finished){
    
    // Push back to queue if process is not completed, if completed,
    // print process and evict memory.
    if(process->rem_time != 0){
        insert_end(queue, process);
    } else{
        if((strcmp(details->m_algo, UNLIMITED) && process->rem_time == 0)){
            evict_memory(memory_arr, pages, process, *time);
        }
        print_finished(time, process, (queue->size));
        insert_end(finished, process);
    }
}

// Allocates memory for virtual memory algorithm
int allocate_memory_v(Process_list *processes, Mem_block *memory_arr,
                      int n_pages, long long int *time,
                      struct Details *details){
    // Current process
    Process *process = processes->head->process;
    int req_pages = (process->memory_size_req)/PAGE_SIZE;

    // Index of available block to allocate to process
    int available_block = available_memory(memory_arr,n_pages, 
                                                VIRTUAL_MIN_PAGES);
    
    // Count of available memory blocks
    int available_count = available_memory_total(memory_arr,n_pages);
    int load_time = 0;

    // if all already alloted, no need to allocate, update load time
    if(id_alloted_count(memory_arr, n_pages, process->process_id)*PAGE_SIZE
                                             == process->memory_size_req){
        process->load_time = 0;
        return 0;
    }

    // If enough space, allocate. Else evict memory required
    if(available_count >= VIRTUAL_MIN_PAGES ||
       available_count >= req_pages){
        req_pages = get_min(req_pages, available_count);
        load_time = allocate_memory_deep_v(process, memory_arr, req_pages,
                                           available_block, time, n_pages);
        process->load_time = load_time;
    } else {
        req_pages = get_min(req_pages, VIRTUAL_MIN_PAGES);

        // Least recently executed eviction (v)
        if(!strcmp(details->m_algo, VIRTUAL_MEMORY)){
            evict_memory_v(memory_arr, n_pages, *time, req_pages, process);
        } 
        // Most recently executed eviction (cm)
        else if(!strcmp(details->m_algo, BYO_M)){
            evict_memory_cm(memory_arr, n_pages, *time, req_pages,
                                                                process);
        } 
        

        int available_block = available_memory(memory_arr,n_pages,
                                                            req_pages);
        load_time = allocate_memory_deep_v(process, memory_arr, req_pages,
                                           available_block, time, n_pages);
        process->load_time = load_time;
    }
    // Update remaining time with pagefault penalty
    process->rem_time += ((process->memory_size_req)/4) - (load_time/2);
    return load_time;
}

// Counts the number of available memory blocks
int available_memory_count(Mem_block *memory_arr, long long int n_pages){
    int count = 0;
    int max_available = 0;
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].process_id == 0){
            count += 1;
        } else{
            if(max_available < count){
                max_available = count;
            }
            count = 0;
        }
    }
    if(max_available < count){
        max_available = count;
    }
    return max_available;
}

// Evicts least recently used memory for virtual memory
void evict_memory_v(Mem_block *memory_arr, long long int size, 
                    long long int current_time, int req_pages, 
                    Process *process){
    
    // If already alloted, dont evict any
    if(!(available_memory_total(memory_arr, size) < get_min(VIRTUAL_MIN_PAGES, req_pages))){
        return;
    }
    int already_alloted = id_alloted_count(memory_arr, size, process->process_id);
    int total_needed = get_min(VIRTUAL_MIN_PAGES, req_pages) - already_alloted;
    if(available_memory_total(memory_arr, size) > total_needed){
        return;
    }

    // Keep track of evicted memory indexes 
    Int_list *used_index = malloc(sizeof(Int_list));
    printf("%lli, EVICTED, mem-addresses=", current_time);

    // Keep evicting until enough memory is free
    while(available_memory_total(memory_arr, size) < total_needed){
        // Find least executed process
        int needed = get_min(req_pages, VIRTUAL_MIN_PAGES) - available_memory_total(memory_arr, size);
        for(int i = 0; i < needed; i++){
            int least_recent = index_of_least_recent(memory_arr, size);
            if(memory_arr[least_recent].process_id != process->process_id){
                memory_arr[least_recent].process_id = 0;
                memory_arr[least_recent].last_executed = 0;
                sorted_insert(used_index, least_recent);
            } else{
                memory_arr[least_recent].last_executed = current_time;
                total_needed--;
            }
        }
    }
    print_list(used_index);
    printf("\n");
}

// Returns available total memory blocks
int available_memory_total(Mem_block *memory_arr, long long int n_pages){
    int count = 0;
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].process_id == 0){
            count++;
        }
    }
    return count;
}

// Evicts most recently used memory, similar to evict_memory_v()
void evict_memory_cm(Mem_block *memory_arr, long long int size,
                     long long int current_time, int req_pages, 
                     Process *process){
    if(!(available_memory_total(memory_arr, size) < 
         get_min(VIRTUAL_MIN_PAGES, req_pages))){
        return;
    }
    int already_alloted = id_alloted_count(memory_arr, size, process->process_id);
    int total_needed = get_min(VIRTUAL_MIN_PAGES, req_pages) - already_alloted;
    if(available_memory_total(memory_arr, size) > total_needed){
        return;
    }
    Int_list *used_index = malloc(sizeof(Int_list));
    printf("%lli, EVICTED, mem-addresses=", current_time);
    
    while(available_memory_total(memory_arr, size) < total_needed){
        // Find most recent executed process
        int needed = get_min(req_pages, VIRTUAL_MIN_PAGES) - 
                            available_memory_total(memory_arr, size);
        for(int i = 0; i < needed; i++){
            int most_recent = index_of_most_recent(memory_arr, size);
            if(memory_arr[most_recent].process_id != process->process_id){
                memory_arr[most_recent].process_id = 0;
                memory_arr[most_recent].last_executed = 0;
                sorted_insert(used_index, most_recent);
            } else{
                memory_arr[most_recent].last_executed = current_time;
                total_needed--;
            }
        }
    }
    print_list(used_index);
    printf("\n");
}

// Returns index of least recently executed memory block
int index_of_least_recent(Mem_block *memory_arr, long long int n_pages){
    int least_recent_time = MAXIMUM_INT;
    int index = -1;
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].last_executed < least_recent_time && 
           memory_arr[i].process_id != 0){
            least_recent_time = memory_arr[i].last_executed;
            index = i;
        }
    }
    return index;
}

// Returns index of most recently executed memory block
int index_of_most_recent(Mem_block *memory_arr, long long int n_pages){
    int most_recent_time = MINIMUM_INT;
    int index = -1;
    for(int i = 0; i < n_pages; i++){
        if(memory_arr[i].last_executed > most_recent_time && 
           memory_arr[i].process_id != 0){
            most_recent_time = memory_arr[i].last_executed;
            index = i;
        }
    }
    return index;
}

// 2nd layer function to allocate memory for virtual memory
int allocate_memory_deep_v(Process *process, Mem_block *memory_arr,
                           int req_pages, int available_block,
                           long long int *time, int size){
    int load_time = 0;
    Int_list *used_index = malloc(sizeof(Int_list));
    int already_alloted = id_alloted_count(memory_arr, size, 
                                                process->process_id);
    process->rem_time -= already_alloted;
    int count = 0 + already_alloted;
    // int total = 0 + already_alloted;
    int start_i = 0;
    for(int i = start_i; count < req_pages || 
        memory_free_exist(memory_arr, size) ; i++){
        if(memory_arr[i].process_id == 0){
            memory_arr[i].process_id = process->process_id;
            memory_arr[i].last_executed = *time;
            insert(used_index, i);
            load_time++;
            count++;
        } 
        if(process->memory_size_req == count*PAGE_SIZE){
            break;
        }
    }
    return load_time*LOAD_TIME;
}

// Checks if there are any free memory space
int memory_free_exist(Mem_block *memory_arr, int size){
    for(int i = 0; i < size; i++){
        if(memory_arr[i].process_id == 0){
            return 1;
        }
    }
    return 0;
}

