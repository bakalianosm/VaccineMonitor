


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

#define MAX_LVL 6
#define INSERT 1
#define NOT_INSERT 0
struct skip_list {
    int size; /* How many elements we have added on the Skip List */
    int curLevel; /* The current level of Skip List */
    int maxLevel; /* Max Skip List Level */
    skipListNode header; /* The header of the Skip List */
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;

};

struct skip_list_node {
    int height;
    Pointer key;
    Pointer value;
    skipListNode *next; /* An array of Skip List Nodes */
};

SkipList SL_create(int maxLevel, DestroyFunc destroy_key, DestroyFunc destroy_value){
    
    /* Allocate the memory that the SkipList needs */
    SkipList skiplist = malloc(sizeof(*skiplist));
    if (skiplist == NULL){
        printf("Error with skip list creation\n");
    }
    
    /* Initialize values */
    skiplist->size = 0;
    skiplist->curLevel = 1;
    skiplist->maxLevel = maxLevel;
    
    skipListNode header = malloc(sizeof(*header));
    header->next = malloc( (maxLevel+1) * sizeof(*(header->next)));
    
    for(int i = 0 ; i<= skiplist->maxLevel ; i++){
        header->next[i] = NULL;
    }


    skiplist->destroy_key = destroy_key;
    skiplist->destroy_value = destroy_value;

    /* Assign the created node to header */
    skiplist->header = header;
    return skiplist;
}

int randomLevel(SkipList skiplist){

    /* This is an internal function that will be used from the 
       insert function */

    /* Its like, how many times we flip a coin and we got tails */
  

    /* At least we have one level */
    float r = (float)rand()/RAND_MAX;
    int level = 1;
    while( r < 0.5 && level < skiplist->maxLevel){
        level++;
        r = (float)rand()/RAND_MAX;
    }

    return level;
}

int SL_size(SkipList skiplist){
    return skiplist->size;
}

int SL_layers(SkipList skiplist){
    return skiplist->curLevel;
}

void SL_insert(SkipList skiplist, Pointer key, Pointer value,CompareFunc compare){

    int height = skiplist->maxLevel +1;

    /* Array of pointer to elements that are predecessors of the new element */
    skipListNode update[height];

    /* Initialize them */
    for(int i = 0 ; i < height ; i++){
        update[i] = NULL;
    }


    skipListNode x = skiplist->header;
    int i;
    
    /* And start from the upper level until we find a node smaller than the given key */
    for (i = skiplist->curLevel; i>=1 ; --i){
        while( x->next[i]!=NULL){
            if( compare(x->next[i]->key , key) >=0 ){
                break;
            }
            x = x->next[i];
        }
        update[i] = x;
    }
    
    if(x==NULL){
        if(x->next[0]!=NULL){
            if (x->next[0]->key!=NULL){
                if (compare(x->next[0]->key,key)== 0 ){
                    /* If exists node with this key, replace its value */
                    x->next[0]->value = value;
                    return ;
                }
            }
        }
    } 
    else {

        /* Flip a coin */
        int level = randomLevel(skiplist);


        if (level > skiplist->curLevel){
            for(int i= skiplist->curLevel +1 ; i<= level ; i++){
                update[i] = skiplist->header;
            }
            skiplist->curLevel = level;
            
        }
        /* Create a new node to be inserted */
        x = malloc(sizeof(*x));
        x->key = key;
        x->value = value;

        x->next = malloc(sizeof(*(x->next))* (level +1));
        
        for (int i = 1 ; i <= level ; i++){
            x->next[i] = NULL;
        }
        for (int i = 1 ; i <= level ; i++){
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
    skiplist->size++;
}

skipListNode SL_find_node(SkipList skiplist, Pointer key, CompareFunc compare){
    
    /* *First node */
    skipListNode x = skiplist->header;

    /* Start from the highest level on the list until we find a node smaller than the given key */
    for (int i = skiplist->curLevel ; i >= 1 ; --i){
        // if(x == NULL){
            while( x->next[i]!=NULL){

                if( compare(x->next[i]->key , key) >=0 ){
                    break;
                }
                x = x->next[i];
            }
        // }
    }

    if(x->next[1]!=NULL){
        if (x->next[1]->key!=NULL){
            if (compare(x->next[1]->key,key)== 0 ){

                return x->next[1];
            }
        }
    }   

    return NULL;

}

Pointer SL_node_key(skipListNode node){
    if(node->key!= NULL){
        return node->key;
    }
    else{
        return NULL;
    }
}


Pointer SL_node_val(skipListNode node){
    if(node->value != NULL){
        return node->value;
    }
    else{
        return NULL;
    }
}


void SL_print(SkipList skiplist, PrintFunc print){
    /* Start from the upper leve */
    for(int i = skiplist->curLevel ; i >=0 ; i--){
        skipListNode node = skiplist->header->next[i];
        // printf("Level %d\n", i);
        /* And print values */
        while(node !=NULL){
            Pointer key = node->key;
            print(key);
            node = node->next[i];
        }
        printf("\n");
    }
}

Pointer SL_find(SkipList skiplist, Pointer key, CompareFunc compare){
    skipListNode node = SL_find_node(skiplist,key,compare);
    return node == NULL ? NULL : node->value;

}

void SL_remove(SkipList skiplist, Pointer key, CompareFunc compare){
    skipListNode x = skiplist->header;
    skipListNode delNode = NULL;

    /* Start from the upper level */
    for (int i = skiplist->curLevel; i>=1 ; --i){
        while( x->next[i]!=NULL){
            if( compare(x->next[i]->key , key) >0 ){
                break;
            }
            else if (compare(x->next[i]->key , key) ==0){
                /* We found the node to be deleted */
                delNode = x->next[i];
                x->next[i] = x->next[i]->next[i];
                break;
            }
            x = x->next[i];
        }
    }

    /* Destroy the node to be deleted */
    if (skiplist->destroy_key != NULL && delNode->key!=NULL){
        skiplist->destroy_key(delNode->key);
    }
    if (skiplist->destroy_value != NULL && delNode->value!=NULL){
        skiplist->destroy_value(delNode->value);
    }

    if (delNode->next != NULL) free(delNode->next);
    if (delNode!= NULL)  free(delNode);
}

void SL_destroy(SkipList skiplist){

    skipListNode node = skiplist->header;
    skipListNode nextNode = NULL;

    /* Destroy all nodes in level 1 */
    while (node != NULL){
        nextNode = node->next[1];
        if (node != skiplist->header){    
            if (skiplist->destroy_key != NULL){
                skiplist->destroy_key(node->key);
            }
            if (skiplist->destroy_value != NULL){
                skiplist->destroy_value(node->value);
            }
        }

        if (node->next != NULL) free(node->next);
        if (node!= NULL)  free(node);
        node = nextNode;
    }
   
    free(skiplist);
}


skipListNode SL_first(SkipList skiplist){
   
   skipListNode node = skiplist->header;
   if(node != NULL) return node->next[1];
   else return NULL;
}

skipListNode SL_next(skipListNode node){
    if (node != NULL)
        return node->next[1];
    else return NULL;
}