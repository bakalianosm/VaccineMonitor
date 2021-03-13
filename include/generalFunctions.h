



/* -------------------------------------------------------
 * --------------- GENERAL FUNCTIONS ---------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once
#include "common.h"

#define NO_ERROR 0
#define AGE_ERR -1
#define VACCINATED_DAY_ERR -2
#define NOT_GIVEN_DATE_ERR -3

/* A function that prints red color text */
void printRed(const char* sentence);

/* A function that prints green color text */
void printGreen(const char* sentence);

/* creates a pointer to the given integer value */
int* create_int(int value);

/* A function that is used to compare two records */
int compare_records(Pointer a, Pointer b);

/* A function that is used to destroy a key of  a Citizen Map */
void record_destroy_key(Pointer value);

/* A function that is used to destroy a value of Map */
void record_destroy_value(Pointer rec);

/* A function that is used to compare two viruses */
int compare_viruses(Pointer a, Pointer b);

/* A function that is used to destroy a key of BF Virus Map */
void destroy_virus(Pointer value);

/* A function that is used to destroy a bloom filter from a map */
void destroy_virus_bf(Pointer rec);


/* A function that parses the values from each sentence from the 
   input file to an array of strings */
void parseValues(char buffer[], char* array[]);

/* A function that set the values of records' data array to values */
ERR_CHK assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, date* dateVaccinated);

/* Creates, initializes and returns a record with the given values */
Record initializeCitizen(int ID,   char *firstName, char *lastName, char *country, int age, char *virusName, char *isVaccinated, date dateVaccinated);

/* Prints the data of a citizen */
void printCitizen(Record citizen);

/* returns the number of  tokes the str has */
int countArgs(char* str);

/* Read user's options */
USR_INPT readUserInput();
