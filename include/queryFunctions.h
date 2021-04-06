


/* -------------------------------------------------------
 * --------------- QUERY FUNCTIONS ---------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

#include "bloomFilter.h"
#include "linkedList.h"
#include "common.h"
#include "map.h"

/* ------------------------------------------ QUERIES ------------------------------------------ */

/* vaccineStatusBloom QUERY */
void vaccineStatusBloom(char str[], char* arr[], Map bloomFilterMap);

/* vaccineStatus QUERY */
void vaccineStatus(int numOfArguments, char str[], char* arr[], Map vaccSkipListMap,Map notVaccSkipListMap, LinkedList virusesList);

/* populationStatus QUERY */
void populationStatus(int numOfArguments, char str[], char* arr[], Map vaccSkipListMap, Map notVaccSkipListMap, Map countryPopulationMap, LinkedList countriesList);

/* popStatusByAge  QUERY */
void popStatusByAge(int numOfArguments, char str[], char* arr[], Map vaccSkipListMap, Map notVaccSkipListMap, Map countryPopulationMap, LinkedList countriesList);

/* insertCitizenRecord QUERY */
void insertCitizenRecord(int numOfArguments,int bloomSize, char str[], char* arr[], Map citizenMap, Map vaccSkipListMap, Map notVaccSkipListMap, Map distinctCitizens, Map bloomFilterMap, Map countryPopulationMap, LinkedList countriesList, LinkedList virusesList );

/* vaccinateNow  QUERY*/
void vaccinateNow(int numOfArguments,int bloomSize, char str[], char* arr[], Map citizenMap, Map vaccSkipListMap, Map notVaccSkipListMap, Map distinctCitizens, Map bloomFilterMap, Map countryPopulationMap, LinkedList countriesList, LinkedList virusesList );

/* listNonVaccinatedPersons QUERY */
void listNonVaccinatedPersons(int numOfArguments, char str[], char* arr[], Map notVaccSkipListMap);


/* --------------------------------- ASSISTANT FUNCTIONS FOR QUERIES --------------------------------- */


/* This function searches for the given citizen in the given Virus Skip List and returns it if exists */
Record searchForCitizenInVaccSkipList(int* searchID, Map vaccSkipListMap, char* virus);

/* This function searches for the given citizen in the given Virus Skip List and returns it if exists */
Record searchForCitizenInNotVaccSkipList(int* searchID,Map notVaccSkipListMap,char* virus);

/* This function is used to find the vaccinated percentage for vaccinated people for the given virus 
   and country in range [FROM, TO]*/
void findCountryPercentage(char* country, char* virus, Date from,  Date to, Map vaccSkipListMap, Map countryPopulationMap);

/* This function is used to find the vaccinated percentage **per age** for vaccinated people for the given virus
   and country  in range [FROM, TO]*/
void findCountryPercentageByAge(char* country, char* virus, Date from,  Date to, int ageArr[], Map vaccSkipListMap, Map countryPopulationMap);

/* This function removes the given citizen from the SkipList for the given virus */
void removeFromSkipList(Map SkipListMap, Record citizen, char* virus);
