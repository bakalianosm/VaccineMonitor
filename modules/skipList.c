


/* --------------------------------------------------------
 * ------------------- SKIP LIST ------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */


/* ----------- System header files  --------- */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* -------- User-defined header files ------- */
#include "common.h"
#include "skipList.h"
#include "linkedList.h"

struct skip_list {
    int size;
    int layers;
    DestroyFunc destroyItem;
    LinkedList *listArray;
};

SkipList SL_create(int estimatedSize, DestroyFunc destroy){
    
    /* allocate memory to store the skip list */
    SkipList skiplist = malloc(sizeof(*skiplist));

    /* initialize values */
    skiplist->size = 0;
    skiplist->layers = log2((double)estimatedSize);
    
    /* after we computed the number of layers we can initialize lists*/
    skiplist->listArray = malloc(skiplist->layers * sizeof(LinkedList));
    for (int i = 0 ; i < skiplist->layers ; i++){
        skiplist->listArray[i] = LL_create(free);
    }

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

void SL_destroy(SkipList skiplist){

    /* iterate through all the lists */
    for (int i = 0 ; i < skiplist->layers ; i++){
        LL_destroy(skiplist->listArray[i]);
    }

    /* free the listArray pointer */
    free(skiplist->listArray);

    /* free the struct skipList */
    free(skiplist);
}


