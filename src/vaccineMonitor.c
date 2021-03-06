


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "skipList.h"
#include "linkedList.h"
#include "bloomFilter.h"
#include "vaccineMonitor.h"
#include "generalFunctions.h"


int* create_int(int value) {
	int* pointer = malloc(sizeof(int));	// δέσμευση μνήμης
	*pointer = value;					// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}



int main(int argc, char *argv[]){
    
    if (argc != 5) exit(EXIT_FAILURE);
    printf("Hello from Vaccine Monitor\n");
    
    /* Arguments parsing */
    char *recordFile;
    int bloomSize;
    if( (strcmp(argv[1], "-c")==0) && (strcmp(argv[3], "-b")==0) ){
            recordFile = strdup(argv[2]);
            bloomSize = atoi(argv[4]);      
        }
    else{
        printf("Invalid command line sequence.Exiting...\n");
        exit(EXIT_FAILURE);
    }

    printf("Arguments are [%s] [%d]\n",recordFile,bloomSize);

    
    LinkedList list = LL_create(free);

    int *p1 = create_int(8);
    int *p2 = create_int(15);
    int *p3 = create_int(3);
    LL_insert_at_start(list,p1);
    LL_insert_at_start(list,p2);
    LL_insert_at_start(list,p3);


    ListNode tempNode;
    for(ListNode node = LL_list_first(list) ; node != NULL ; node = LL_list_next(list,node)){
        int *val = LL_node_val(node);
        if(*val==15) tempNode = node;
        printf("node val = %d\n", *val);
    }

    printf("Inserting 16\n");
    // insert next of 15
    int *p4 = create_int(16);
    LL_insert_after(list,tempNode,p4);

    for(ListNode node = LL_list_first(list) ; node != NULL ; node = LL_list_next(list,node)){
        int *val = LL_node_val(node);
        if(*val==15) tempNode = node;
        printf("node val = %d\n", *val);
    }

    LL_destroy(list);
}