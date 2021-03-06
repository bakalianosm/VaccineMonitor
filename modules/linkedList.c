


/* --------------------------------------------------------
 * ------------------- LINKED LIST ------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */


/* ----------- System header files  --------- */
#include <stdlib.h>
#include <stdio.h>

/* -------- User-defined header files ------- */
#include "common.h"
#include "linkedList.h"



struct linked_list { 
    int size;
    ListNode dummy;
    ListNode last;
    DestroyFunc destroyValue;
};

struct list_node {
    Pointer value;
    ListNode next;
};

LinkedList LL_create(DestroyFunc destroy){ 
    LinkedList list= malloc(sizeof(*list));
    list->size = 0;
    list->destroyValue = destroy;

    /* Add dummy node, next of dummy will be null because the list is empty */
    list->dummy = malloc(sizeof(*list->dummy));
    list->dummy->next = NULL;

    /* set the last node */
    list->last = list->dummy;

    return list;
}

int LL_size(LinkedList list){
    return list->size;
}
void LL_insert_at_start(LinkedList list, Pointer value){

    /* Create a new node */
    
    ListNode newNode = malloc(sizeof(*newNode));
    newNode->value = value;

    /* Set the new node as the next of the dummy */
    newNode->next =  list->dummy->next;
    list->dummy->next = newNode;

    
    /* Increment the list size */
    if(list->size == 0 )
        list->last = newNode;

    list->size++;
}

void LL_insert_after(LinkedList list, ListNode node, Pointer value){
    /* Create a new node */

    if (node == NULL)
        node=list->dummy;
        
    ListNode newNode = malloc(sizeof(*newNode));
    newNode->value = value;

    /* Set the new node as the next of the given */
    newNode->next =  node->next;
    node->next = newNode;

    /* Check if the node is the last */
    if(list->last == node)
        list->last = newNode;
    /* Increment the list size */
    list->size++;


}

void LL_insert_ordered(LinkedList list, Pointer value, CompareFunc compare){
	ListNode prev = NULL;

	/* If list is empty then we have to insert at the start */
	if(LL_size(list)==0) {
		LL_insert_at_start(list,value);
	}
	else {
		for(ListNode node = LL_first(list) ; node!= NULL ; node = LL_next(node)){
			if(compare(value,LL_node_val(node))<=0){
				break;
			}
			prev = node;
		}
		LL_insert_after(list,prev,value);
	}
}
void LL_remove_next_item(LinkedList list, ListNode node){

    if (node == NULL)
        node = list->dummy;

    ListNode toRemove = node->next;

    /* Destroy the value of a node */
    if (list->destroyValue != NULL)
        list->destroyValue(toRemove->value);
    
    node->next = toRemove->next;
    free(toRemove);

    list->size--;

    /* If the node to be removed is the last */
    if (list->last == toRemove)
        list->last = node;

}

Pointer LL_find(LinkedList list, Pointer value, CompareFunc compare){
    ListNode node = LL_find_node(list,value,compare);
    return node == NULL ? NULL : node->value;
}

void LL_destroy(LinkedList list){
    ListNode node = list->dummy;
    /* Iterate trough the list and free the nodes */ 
    while (node != NULL){
        ListNode next = node->next;

        if (node != list->dummy && list->destroyValue != NULL)
            list->destroyValue(node->value);
        free(node);
        node = next;
    }

    /* Destroy the struct */
    free(list);
}

ListNode LL_first(LinkedList list){
    /* The next of dummy is the next */
    return list->dummy->next;
}

ListNode LL_last(LinkedList list){
    if(list->last == list->dummy)
        return NULL ;
    else return list->last ;

}

ListNode LL_next(ListNode node){
    if(node != NULL )
        return node->next;
    else
        return NULL ;

}


Pointer LL_node_val(ListNode node){
    if(node != NULL)
        return node->value;
    else return NULL;
}

ListNode LL_find_node(LinkedList list, Pointer value, CompareFunc compare){
    
    /* Iterate through the list */
    for (ListNode node = list->dummy->next ; node != NULL; node=node->next)
        if (compare(value, node->value) == 0)
            return node;
    
    return NULL;
}
