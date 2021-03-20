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

    SkipList sl = SL_create(6, free, free);
    // printf("hello from skip list test\n");

    int N = 10;
	int** key_array = malloc(N * sizeof(*key_array));
	int** value_array = malloc(N * sizeof(*value_array));
    for (int i = 0; i < N; i++) {
		key_array[i] = create_int(i);
		value_array[i] = create_int(i);

		SL_insert(sl ,key_array[i], value_array[i],compare_ints);
		// Ανά τακτά χρονικά διαστήματα διαγράφουμε κάποιο κλειδί που μόλις βάλαμε
		if (i % 2 == 0) 
			SL_remove(sl, key_array[i],compare_ints);
	}
    // LinkedList ll = LL_create(NULL);

    
    // for (ListNode l = LL_list_first(ll); l !=NULL ; l = LL_list_next(ll,l)){
    //     int * val = LL_node_val(l);
    //     printf("node is %d\n", *val);
    // }
    // ListNode last = LL_list_last(ll);
    // int *val  = LL_node_val(last);
    // printf("list last is %d\n", *val);


    // printf("Estimated layers are %d \n", SL_layers(skip));
    SL_print(sl,print_int);

    SL_destroy(sl);
    free(key_array);
	free(value_array);
    // LL_destroy(ll);
   
    // int arr[] = {6,2,3,9,15,1,22,45,96,27,14,110,123,125,129,444,3445,4874,654,987,44};

    // for (int i = 0 ; i < 21 ; i++){
    //     SL_insert(sl,&arr[i], &arr[i], compare_ints);
    // }

    // SL_insert(sl, create_int(5000),create_int(5000),compare_ints);
    //  SL_insert(sl, create_int(3000),create_int(3000),compare_ints);
    // BloomFilter bf = bf_create(16,1000, hash_i);
    // SL_print(sl,print_int);
    // bf_insert(bf, "MICHALIS");
    // bf_insert(bf, "THANASIS");



    // printRed("\t\t REMOVING ITEM \n");
    // int *key_ar[11];
    // for (int i = 0 ; i < 11 ; i++){
    //     key_ar[i] = create_int(arr[i]);
    //     SL_remove(sl, key_ar[i], compare_ints);
    // }

    // SL_destroy(sl);
    // bf_insert(bf, "KOSTAS");
    // bf_insert(bf, "MICHALIS");
  
    // skipListNode node = SL_find_node(sl,&arr[4],compare_ints);
    // if (node==NULL) printf("null node\n");
    // int *key = SL_node_key(node);
    // int *val = SL_node_val(node);
    // printf("node val is %d\t%d", *val, *key);
    // // bf_search(bf,"KOSTAS");
    // bf_search(bf,"THANASI");
   
    // bf_destroy(bf);

    // SkipList sl = SL_create(16, NULL);
    // if (sl != NULL ) printf("Skip List created with %d layers\n",SL_layers(sl));

    // int arr[] = {1,5,6,8,10,15,20,105,15,58};

    // for (int i = 0 ; i <10 ; i ++){
    //     SL_insert(sl, &arr[i], compare_ints);
    // }

    // SL_print(sl);

    // Map dokimastiko = map_create(compare_viruses,NULL,NULL);
    // map_set_hash_function(dokimastiko, hash_string);
    // char* temp1 = strdup("ena");
    // char* temp2 = strdup("dyo");
    // char* temp3 = strdup("tria");


    // map_insert(dokimastiko, temp1, temp1);
    // map_insert(dokimastiko, temp2, temp2);
    // map_insert(dokimastiko, temp3, temp3);

    // MapNode m = MAP_EOF;
    // m = map_find(dokimastiko ,temp1);

    // if(m==MAP_EOF) printf("Den\n");
}