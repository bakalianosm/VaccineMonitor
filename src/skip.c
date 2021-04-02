#include "skipList.h"
#include "linkedList.h"
#include <stdio.h>
#include "map.h"
#include "bloomFilter.h"
#include "generalFunctions.h"
#include <string.h>
#include <stdlib.h>


int compare_ints(Pointer a, Pointer b) {
    return *(int*)a - *(int*)b;
}

void print_int(Pointer a){
    printf("%d\t", *(int*)a);
}
int main(void){

    // SkipList sl = SL_create(6, free, free);
    // printf("hello from skip list test\n");

    // int N = 10;
	// int** key_array = malloc(N * sizeof(*key_array));
	// int** value_array = malloc(N * sizeof(*value_array));
    // for (int i = 0; i < N; i++) {
	// 	key_array[i] = create_int(i);
	// 	value_array[i] = create_int(i);

	// 	SL_insert(sl ,key_array[i], value_array[i],compare_ints);

        
		// Ανά τακτά χρονικά διαστήματα διαγράφουμε κάποιο κλειδί που μόλις βάλαμε
		// if (i % 2 == 0) 
		// 	SL_remove(sl, key_array[i],compare_ints);
	// }
    // LinkedList ll = LL_create(NULL);

    
    // for (ListNode l = LL_list_first(ll); l !=NULL ; l = LL_list_next(ll,l)){
    //     int * val = LL_node_val(l);
    //     printf("node is %d\n", *val);
    // }
    // ListNode last = LL_list_last(ll);
    // int *val  = LL_node_val(last);
    // printf("list last is %d\n", *val);
    // int * k = create_int(2);
    // skipListNode n1 = SL_find_node(sl, k, compare_ints);
    //     if( n1 != NULL){
    //         int * val =  SL_node_val(n1);
    //         printf("for key 2 value is %d\n", *val);
    //     }
    //     else {
    //         printf("dit not found\n");
    //     }

    // printf("Estimated layers are %d \n", SL_layers(skip));

    // for(skipListNode node = SL_first(sl) ; node != NULL ; node = SL_next(node)){
    //     int* key = SL_node_key(node);
    //     int* value = SL_node_val(node);
    //     printf("key is %d val is %d\n",*key, *value);
    // }
    // SL_print(sl,print_int);
    // free(k);
    // SL_destroy(sl);

    // free(key_array);
    // free(value_array);



    
}