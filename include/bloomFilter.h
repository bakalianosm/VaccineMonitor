



/* -------------------------------------------------------
 * ----------------- BLOOM FILTER ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "common.h"

typedef struct bloom_filter* BloomFilter;
typedef uint (*HashFunc)(Pointer);

/* Initializes and creates an empty Bloom Filter */
BloomFilter bf_create(int size, HashFunc HF1, HashFunc HF2, HashFunc HF3);