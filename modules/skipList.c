


/* --------------------------------------------------------
 * ------------------- SKIP LIST ------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */


/* ----------- System header files  --------- */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
/* -------- User-defined header files ------- */
#include "common.h"
#include "skipList.h"
#include "linkedList.h"


#define INSERT 1
#define NOT_INSERT 0
struct skip_list {
  
    int size;
    int layers;
    LinkedList bottomList;
    LinkedList* upperLists;
    DestroyFunc destroyItem;
    
};

struct upper_node {
    UpperNode up;
    UpperNode next;
    ListNode down;
};

SkipList SL_create(int estimatedSize, DestroyFunc destroy){
    
    /* allocate memory to store the skip list */
    SkipList skiplist = malloc(sizeof(*skiplist));

    /* initialize values */
    skiplist->size = 0;
    skiplist->layers = log2((double)estimatedSize);
    
    /* after we computed the number of layers we can initialize lists*/
    
    skiplist->destroyItem = destroy;

    return skiplist;
}

int SL_size(SkipList skiplist){
    return skiplist->size;
}

int SL_layers(SkipList skiplist){
    return skiplist->layers;
}

void SL_insert(SkipList skiplist, Pointer value, CompareFunc compare){

}

void SL_print(SkipList skiplist){
    return;
}
void SL_destroy(SkipList skiplist){
    return;
}


