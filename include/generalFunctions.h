


/* -------------------------------------------------------
 * --------------- GENERAL FUNCTIONS ---------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "bloomFilter.h"
#include "linkedList.h"
#include "common.h"
#include "map.h"

#define NO_ERROR 0
#define AGE_ERR -1
#define VACCINATED_DAY_ERR -2
#define NOT_GIVEN_DATE_ERR -3


/* a hash function that hases the id and the virus of a record */
uint hash_vaccine(Pointer value);

/* A function that prints red color text */
void printRed(const char* sentence);

/* A function that prints green color text */
void printGreen(const char* sentence);

/* creates a pointer to the given integer value */
int* create_int(int value);

/* A function that is used to compare two hash structs  */
int compare_keys(Pointer a, Pointer b);

/* A function that is used to compare citizens (useless) */ 
int compare_values(Pointer a, Pointer b);

/* A function that is used to compare citizens */ 
int compare_IDs(Pointer a, Pointer b);

/* A function that is used to destroy a key of  a Citizen Map */
void record_destroy_key(Pointer value);

/* A function that is used to destroy a value of Map */
void record_destroy_value(Pointer rec);

/* A function that is used to compare two viruses */
int compare_viruses(Pointer a, Pointer b);

/* A function that is used to compare two countries */
int compare_countries(Pointer a, Pointer b);

/* A function that is used to destroy a key of BF Virus Map */
void destroy_virus(Pointer value);

/* A function that is used to destroy a bloom filter [value] from a map */
void destroy_virus_bf(Pointer rec);

/* A function that is used to destroy a skiplist [value] from a map */
void destroy_vacc_skip_list(Pointer value);

/* A function that parses the values from each sentence from the
   input file to an array of strings */
void parseValues(char buffer[], char* array[]);

/* A function that set the values of records' data array to values */
ERR_CHK assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, char **dateVaccinated);

ERR_CHK assignValuesForIoption(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, char **dateVaccinated);

/* Creates a struct that is the values that gonna be hashed */
hashRec initializeHashKey(int ID,char* virus);

/* Creates, initializes and returns a record with the given values */
Record initializeCitizen(int ID,   char *firstName, char *lastName, char *country, int age, char *virusName, char *isVaccinated, char* dateVaccinated);

/* Prints the data of a citizen */
void printCitizen(Record citizen);

/* returns the number of  tokes the str has */
int countArgs(char* str);

/* checks if the given ID is valid for the program */
int checkID(char str[]);

/* checks if the given date is valid for the program */
int checkDate(Date date);

/* removes the char "toRem" from the str if exists */
void removeChar(char *str, char toRem);



/* Read user's options */
USR_INPT readUserInput(int bloomSize, Map bfMap, Map vaccSkipListMap, Map notVaccSkipListMap, LinkedList virusesList, Map countryPopulationMap, LinkedList countriesList, Map citizenMap, Map distinctCitizens);
