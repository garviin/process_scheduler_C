/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for integer linkedlist, please refer to .c file for
 *          documentation of each function.
 */
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Linkedlist of ints
typedef struct Int_list {
    struct Int_node *head;
    struct Int_node *tail;
    int size;
} Int_list;

// A node for int linked list
typedef struct Int_node {
    int data;
    struct Int_node *next;
    struct Int_node *prev;
} Int_node;

void insert(Int_list *list, int new);
void print_list(Int_list *list);
void sorted_insert(Int_list *list, int new);

#endif