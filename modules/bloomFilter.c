
/* --------------------------------------------------------
 * ------------------- BLOOM FILTER -----------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* -------- User-defined header files ------- */
#include "bloomFilter.h"


struct bloom_filter {
    int size;
    int *array;
    HashFunc firstHF;
    HashFunc secondHF;
    HashFunc thirdHF;
};

BloomFilter bf_create(int size, HashFunc HF1, HashFunc HF2, HashFunc HF3){
    BloomFilter bloomfilter= malloc(sizeof(*bloomfilter));
    assert(bloomfilter != NULL);

    bloomfilter->size = 0;
    bloomfilter->array = malloc((100/32 +1)* sizeof(int));
    bloomfilter->firstHF = HF1;
    bloomfilter->secondHF = HF2;
    bloomfilter->thirdHF = HF3;
    
    return bloomfilter;
}