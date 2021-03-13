


/* -------------------------------------------------------
 * ------------------ SKIP LIST ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "common.h"

typedef struct skip_list* SkipList;
typedef struct upper_node* UpperNode;

/* This function creates and empty SkipList */
SkipList SL_create(int estimatedSize, DestroyFunc destroy);

/* This function is used to return the number of layers */
int SL_layers(SkipList skiplist);

/* This function is used to return the size of the skiplist */
int SL_size(SkipList skiplist);

/* This function is used to insert an item in skip list */
void SL_insert(SkipList skiplist, Pointer value, CompareFunc compare);

/* This function is used to print each level of the skip list */
void SL_print(SkipList skiplist);

/* This function is used to free all the memory that the list has allocated */
void SL_destroy(SkipList skiplist);