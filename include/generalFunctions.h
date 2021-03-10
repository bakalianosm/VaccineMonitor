



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

/* A function that parses the values from each sentence from the 
   input file to an array of strings */
void parseValues(char buffer[], char* array[]);

/* A function that set the values of records' data array to values */
void assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, date* dateVaccinated);