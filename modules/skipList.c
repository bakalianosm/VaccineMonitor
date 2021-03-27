


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
    int size;
    int curLevel;
    int maxLevel;
    skipListNode header;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;

};

struct skip_list_node {
    int height;
    Pointer key;
    Pointer value;
    skipListNode *next;
};

SkipList SL_create(int maxLevel, DestroyFunc destroy_key, DestroyFunc destroy_value){
    
    SkipList skiplist = malloc(sizeof(*skiplist));
    if (skiplist == NULL){
        printf("Error with skip list creation\n");
    }
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

    skiplist->header = header;
    return skiplist;
}

int randomLevel(SkipList skiplist){

    /* This is an internal function that will be used from the 
       insert functio */

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

    /* array of pointer to elements that are predecessors of the new element */
    int height = skiplist->maxLevel +1;

    skipListNode update[height];

    for(int i = 0 ; i < height ; i++){
        update[i] = NULL;
    }


    skipListNode x = skiplist->header;
    int i, level;
    
    for (i = skiplist->curLevel; i>=1 ; --i){
        while( x->next[i]!=NULL){
            if( compare(x->next[i]->key , key) >=0 ){
                break;
            }
            x = x->next[i];
        }
        update[i] = x;
    }
    
    if(x==NULL){ // borei na to bvgalo
        if(x->next[0]!=NULL){
            if (x->next[0]->key!=NULL){
                if (compare(x->next[0]->key,key)== 0 ){
                    x->next[0]->value = value;
                    return ;
                }
            }
        }
    } // mporei na to vgalo
    else {
        int level = randomLevel(skiplist);

        if (level > skiplist->curLevel){
            for(int i= skiplist->curLevel +1 ; i<= level ; i++){
                update[i] = skiplist->header;
            }
            skiplist->curLevel = level;
            
        }

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
    
    /* first node */

    skipListNode x = skiplist->header;

    /* start from the highest level on the list */
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
    for(int i = skiplist->curLevel ; i >=0 ; i--){
        skipListNode node = skiplist->header->next[i];
        // printf("Level %d\n", i);
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

    for (int i = skiplist->curLevel; i>=1 ; --i){
        while( x->next[i]!=NULL){
            if( compare(x->next[i]->key , key) >0 ){
                break;
            }
            else if (compare(x->next[i]->key , key) ==0){
                delNode = x->next[i];
                x->next[i] = x->next[i]->next[i];

                // free(delNode);
                break;
            }
            x = x->next[i];
        }
    }

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