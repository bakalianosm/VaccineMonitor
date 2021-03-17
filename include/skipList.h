


/* -------------------------------------------------------
 * ------------------ SKIP LIST ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "common.h"

typedef struct skip_list* SkipList;
typedef struct skip_list_node* skipListNode;

/* This function creates and empty SkipList */
SkipList SL_create(int maxLevel, DestroyFunc destroy_key, DestroyFunc destroy_value);

/* This function is used to return the number of layers */
int SL_layers(SkipList skiplist);

/* This function is used to return the size of the skiplist */
int SL_size(SkipList skiplist);

/* This function is used to insert an item in skip list */
void SL_insert(SkipList skiplist, Pointer key, Pointer value, CompareFunc compare);

/* This function is used to print each level of the skip list */
void SL_print(SkipList skiplist, PrintFunc print);

/* IF node exist returns value of the first node that has "key" key based on  the compare function */
Pointer SL_find(SkipList skiplist, Pointer key, CompareFunc compare);

/* Removes node with "key" key if exists */
void SL_remove(SkipList skiplist, Pointer key,CompareFunc compare);

/* This function is used to free all the memory that the list has allocated */
void SL_destroy(SkipList skiplist);

/* Returns the first node with the given "key" base on the compare function */
skipListNode SL_find_node(SkipList skiplist, Pointer key, CompareFunc compare);

/* Returns the key of the node */
Pointer SL_node_key(skipListNode node);

/* Returns the value of the node */
Pointer SL_node_val(skipListNode node);
