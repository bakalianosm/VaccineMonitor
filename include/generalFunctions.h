



/* -------------------------------------------------------
 * --------------- GENERAL FUNCTIONS ---------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once
#include "common.h"

/* A function that is used to compare two records */
int compare_records(Pointer a, Pointer b);

/* A function that is used to destroy a key of Map */
void record_destroy_key(Pointer rec);

/* A function that is used to destroy a value of Map */
void record_destroy_value(Pointer rec);