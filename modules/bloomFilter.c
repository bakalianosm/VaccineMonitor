
/* --------------------------------------------------------
 * ------------------- BLOOM FILTER -----------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* -------- User-defined header files ------- */
#include "bloomFilter.h"


struct bloom_filter {
    int numK;
    int size;
    int sizeOfItems;
    char *array;
    BF_HashFunc HF;
   
};

BloomFilter bf_create(int k, int size, BF_HashFunc hashfunction){
    BloomFilter bloomfilter= malloc(sizeof(*bloomfilter));

    bloomfilter->numK = k;
    bloomfilter->size = size;
    bloomfilter->sizeOfItems = 0;
    bloomfilter->array = malloc(size*sizeof(char));
    
    /* Initialize array with 0 */
    memset(bloomfilter->array, 0, size*sizeof(char));

    bloomfilter->HF = hashfunction;

    
    return bloomfilter;
}

int bf_size(BloomFilter bloomfilter){
    return bloomfilter->size;
}

int bf_items(BloomFilter bloomfilter){
    return bloomfilter->sizeOfItems;
}

int bf_numK(BloomFilter bloomfilter){
    return bloomfilter->numK;
}

void bf_insert(BloomFilter bloomfilter, unsigned char* str){
    
    for (int i = 0 ; i < bloomfilter->numK ; i++){

        /* Hash the given string */
        unsigned long retVal = bloomfilter->HF(str, i);
        /* printf("hash is %lu\t", retVal); */

        int num = retVal % (bloomfilter->size * 8 *sizeof(char));
        
        /* Find which byte to change */
        int byte =  num/8;

        /* Find which bit on byte to change  */
        int bitOnByte = num%8;
       
        /* Change bit value */
        bloomfilter->array[byte] |= 1UL << bitOnByte;
    }

    bloomfilter->sizeOfItems++;
}

int bf_search(BloomFilter bloomfilter, unsigned char* str){
   
    for (int i = 0 ; i < bloomfilter->numK ; i++){
        unsigned long retVal = bloomfilter->HF(str, i);

        int num = retVal % (bloomfilter->size * 8 *sizeof(char));
        int byte =  num/8;
        int bitOnByte = num%8;

        /* printf("Bit is %u \n", num); */
        /* printf("This bit is on %d byte ", byte ); */
        /* printf("\tAnd this bit is on %d bit\n", bitOnByte); */
        int bit = (bloomfilter->array[byte] >> bitOnByte) & 1U;
        
        /* printf("bit value is %d\n", bit); */

        /* If the bit's value is 0 the element is not in the Bloom Filter */
        if(bit==0)
            return 0;
    }

    return 1;
}
void bf_destroy(BloomFilter bloomfilter){
    /* Destroy the interior array */
    free(bloomfilter->array);

    /* Destroy the Bloom Filter */
    free(bloomfilter);

}


/* --------------------------------------------------------
 * ------------------- HASH FUNCTIONS ---------------------
 * --------------------------------------------------------
 */

ulong djb2(unsigned char* value) {
    uint hash = 5381;
    for (unsigned char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;		
    return hash;
}

ulong sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

ulong hash_i(unsigned char *str, unsigned int i) {
	return djb2(str) + i*sdbm(str) + i*i;
}