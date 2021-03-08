


/* -------------------------------------------------------
 * ------------------ LINKED LIST ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "common.h"

typedef struct linked_list* LinkedList;
typedef struct list_node* ListNode;

/* This function creates and empty Linked List */
LinkedList LL_create(DestroyFunc destroy);

/* This function is used to return the size of the Linked List */
int LL_size(LinkedList list);
/* This function is used to insert node with "value" in the
    start of the list */
void LL_insert_at_start(LinkedList list, Pointer value);

/* This function is used to insert after a node in list */
void LL_insert_after(LinkedList list, ListNode node, Pointer value);

/* This function is used to create an ordered list */
void LL_insert_ordered(LinkedList list, Pointer value, CompareFunc compare);
/* This function is used to remove "node" node from the list */
void LL_remove_next_item(LinkedList list, ListNode node);

/* Returns the first node that has "value" value based on  the compare function */
Pointer LL_find(LinkedList list, Pointer value, CompareFunc compare);

/* This function is used to free all the memory that the list has allocated */
void LL_destroy(LinkedList list);

/* list iteration */
ListNode LL_list_first(LinkedList list);
ListNode LL_list_last(LinkedList list);
ListNode LL_list_next(LinkedList list, ListNode node);

/* Returns the first node that has "value" value based on  the compare function */
ListNode LL_find_node(LinkedList list, Pointer value, CompareFunc compare);

/* Returns the value of the node */
Pointer LL_node_val(ListNode node);