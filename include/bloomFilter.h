



/* -------------------------------------------------------
 * ----------------- BLOOM FILTER ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "common.h"

typedef struct bloom_filter* BloomFilter;

/* Initializes and creates an empty Bloom Filter */
BloomFilter bf_create(int k, int size, BF_HashFunc hashfunction);

/* Returns the size of the Bloom Filter */
int bf_size(BloomFilter bloomfilter);

/* Returns the number of elements that are in the Bloom Filter */
int bf_items(BloomFilter bloomfilter);

/* Retunrns the value of K */
int bf_numK(BloomFilter bloomfilter);

/* Inserts an element into the Bloom Filter */
void bf_insert(BloomFilter bloomfilter, unsigned char* str);

/* Searches for an element in the Bloom Filter */
int bf_search(BloomFilter bloomfilter, unsigned char* str);

/* Destroys an existing Bloom Filter */
void bf_destroy(BloomFilter bloomfilter);

/* Hash function that will be used */
ulong hash_i(unsigned char *str, unsigned int i);