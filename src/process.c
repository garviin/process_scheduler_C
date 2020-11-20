#include "../include/process.h"

// Reading processes in a file
Process_list *read_processes(char *filename){
    FILE *fp;
    fp = fopen(filename, "r");
    Process_list *processes = malloc(sizeof(Process_list));
    Process *new_process = malloc(sizeof(Process));
    while (EOF != fscanf(fp, "%lli %lli %lli %lli", &(new_process->time_arrived),
                                            &(new_process->process_id),
                                            &(new_process->memory_size_req),
                                            &(new_process->job_time))){
        insert_end(processes, new_process);
        new_process->rem_time = new_process->job_time;
        new_process->load_time = 0;
        new_process = malloc(sizeof(Process));
    }
    return processes;
}

// Insert into beginning of list
void insert_beginning(Process_list *list, Process *new_process) {
    Node *new_node;
    Node *head = list->head;

    new_node = (Node*)malloc(sizeof(Node));
    new_node->process = new_process;

    if (head == NULL) {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->tail = new_node;
    } else{
        new_node->next = head;
        head->prev = new_node;
    }

    list->head = new_node;
    list->size++;
}

// INsert into end of list
void insert_end(Process_list *list, Process *new_process) {
    Node *new_node;
    Node *tail = list->tail;

    new_node = (Node*)malloc(sizeof(Node));
    new_node->process = new_process;

    if (tail == NULL) {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->head = new_node;
    } else{
        tail->next = new_node;
        new_node->prev = tail;
    }

    list->tail = new_node;
    list->size++;
}

// Removes the head of a list
void remove_head(Process_list *list){
    if(list->size == 1){
        list->head = NULL;
        list->tail = NULL;
        list->size--;
        return;
    }
    list->head = list->head->next;
    list->head->prev = NULL;
    list->size--;
}

// Prints processes from the tail
void print_processes_tail(Process_list *list){
    Node *next = list->tail;
    while(next != NULL){
        next = next->prev;
    }
}

// Find process in a list of processes
Process *find_process(Process_list *list, long long int id){
    Node *current = list->head;
    while(current != NULL){
        if(current->process->process_id == id){
            return current->process;
        }
        current = current->next;
    }
    return NULL;
}

// Sorts processes by job time
void sort_processes(Process_list *list)
{
    Node *head = list->head;
    Process *a;

    Node *temp1;
    Node *temp2;

    for(temp1=head;temp1!=NULL;temp1=temp1->next)
      {
        for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next)
          { 
            if(temp2->process->job_time < temp1->process->job_time)
              {
                a = temp1->process;
                temp1->process = temp2->process;
                temp2->process = a;
              }
           }
       }
}