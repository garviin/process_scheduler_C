/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for process struct, please refer to .c file for
 *          documentation of each function.
 */

#include "../include/linkedlist.h"

// Inserts an integer into list
void insert(Int_list *list, int new) 
{ 
    Int_node *new_node;
    Int_node *tail = list->tail;

    new_node = (Int_node*)malloc(sizeof(Int_node));
    new_node->data = new;

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

// Inserts integer into sorted list
void sorted_insert(Int_list *list, int new) 
{ 
    Int_node *current; 
    Int_node *head = list->head;

    Int_node *new_node;
    new_node = (Int_node*)malloc(sizeof(Int_node));
    new_node->data = new;

    /* Special case for the head end */
    if (head == NULL || head->data >= new_node->data) 
    { 
        new_node->next = head; 
        list->head = new_node; 
    } 
    else
    { 
        /* Locate the node before the point of insertion */
        current = head; 
        while (current->next!=NULL && 
               current->next->data < new_node->data) 
        { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 

// Prints all the integers in the linkedlist
void print_list(Int_list *list){
    Int_node *current = list->head;
    printf("[");
    while(current != NULL){
        printf("%d", current->data);
        if(current->next != NULL){
            printf(",");
        }
        current = current->next;
    }
    printf("]");
}
