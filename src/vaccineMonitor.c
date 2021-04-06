


/* --------------------------------------------------------
 * ----------------- VACCINE MONITOR ----------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ---------------- Create Constants -------- */
#define K_FOR_BF 16
#define MAX_SKIPLIST_LEVEL 6

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* -------- User-defined header files ------- */
#include "map.h"
#include "common.h"
#include "skipList.h"
#include "linkedList.h"
#include "unserInput.h"
#include "bloomFilter.h"
#include "queryFunctions.h"
#include "generalFunctions.h"


int main(int argc, char *argv[]){

    
    if (argc != 5) exit(EXIT_FAILURE);

    /* ----------------------------- VARIABLES DECLARATION-------------------------------------- */
    int bloomSize;
    char* recordFile;
    /* Define a file pointer */
    FILE* input_fp ;
    LinkedList virusesList = NULL, countriesList = NULL;
    Map citizenMap = NULL, distinctCitizens = NULL, bloomFilterMap = NULL, vaccSkipListMap = NULL, notVaccSkipListMap = NULL, countryPopulationMap = NULL;
    
    /* ---------------------------- READ COMMAND LINE ARGUMENTS ---------------------------- */
    if( (strcmp(argv[1], "-c")==0) && (strcmp(argv[3], "-b")==0) ){
            recordFile = strdup(argv[2]);
            bloomSize = atoi(argv[4]);
        }
    else{
        printf("Invalid command line sequence.Exiting...\n");
        exit(EXIT_FAILURE);
    }

    /* ------------------------------ OPEN INPUT FILE ---------------------------------------- */
    /* open the file */
    if (recordFile != NULL) input_fp = fopen(recordFile, "r");
    /* Check */
    if (input_fp == NULL){
        perror("Error opening record file ");
    }

    printf("\nWelcome to Vaccine Monitor\n");
    /* --------------------------------- DATA STRUCTURES INITIALIZATION ----------------------------- */
    initialiazeMonitorDataStructures(&virusesList, &countriesList, &distinctCitizens, &citizenMap, &bloomFilterMap, &vaccSkipListMap, &notVaccSkipListMap, &countryPopulationMap);
    /* string buffer for each line */
    char buffer[BUFFER_SIZE];

    /* ---------------------------------- DATA PARSING ------------------------------------------------ */
    char *array[9] = {NULL, NULL, NULL , NULL , NULL, NULL , NULL, NULL, NULL} ;
    while( fgets(buffer,sizeof(buffer),input_fp) != NULL ){

        int parsedID = -1, parsedAge = -1  ;
        char *parsedFirstName = NULL, *parsedLastName = NULL, *parsedCountry = NULL, *parsedVirusName = NULL, *parsedIsVaccinated = NULL, *parsedDateVaccinated = NULL ; 
       /* this array represents an array of the values of input line */


        /* parse the values from the buffer and make an array of these */
        parseValues(buffer,array);

        /* assign the values of the array to the correct variables and validate them */
        ERR_CHK error = assignValues(array,&parsedID, &parsedFirstName, &parsedLastName, &parsedCountry, &parsedAge, &parsedVirusName, &parsedIsVaccinated, &parsedDateVaccinated);

        /* if the above function return an error code , we have to ignore this record */
        if(error != NO_ERROR){
            destroyAssistantValues(parsedFirstName, parsedLastName, parsedCountry, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
            continue;
        } 

        /* create a new record */
        Record citizen = initializeCitizen(parsedID, parsedFirstName, parsedLastName, parsedCountry, parsedAge, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
        /* the hash will be a combination of ID and virus */
        hashRec hashKey = initializeHashKey(citizen->ID, citizen->virusName);

        /* ----------------------------- add new citizen into the Map of Citizens  ---------------------------------- */
        
        map_insert(citizenMap, hashKey, citizen);

        /* ------------------------- update the population of the new citizen's [COUNTRY] -------------------------- */
        updatePopulation(distinctCitizens, countryPopulationMap, citizen);

        /* update with the new [VIRUS] and [COUNTRY] the corresponding Lists.*/
        updateVirusList(virusesList, citizen);
        updateCountyList(countriesList, citizen);
               
        if(strcmp(citizen->isVaccinated,"YES")==0 || strcmp(citizen->isVaccinated,"YES\n")==0 ){
            // printf("%d IS VACCINATED FOR %s\n",citizen->ID,citizen->virusName);

            /* ------------ vaccinated record insert on the Bloom Filter for the citzen's [VIRUS] -------------------- */
            insertCitizenInBloomFilter(bloomSize, bloomFilterMap, citizen);
        
            /* -------- vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS]--------------- */
            insertCitizenInVaccSkipList(vaccSkipListMap, citizen);
        }
        else if(strcmp(citizen->isVaccinated,"NO\n")==0 || strcmp(citizen->isVaccinated,"NO")==0){

            /* ------- *NOT* vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS] --------- */
            insertCitizenInNotVaccSkipList(notVaccSkipListMap, citizen);
        }
        
        // printCitizen(citizen);

        destroyAssistantValues(parsedFirstName, parsedLastName, parsedCountry, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);

       

    }

    
    /* read user's input from keyboard */
    readUserInput(input_fp ,recordFile, bloomSize, bloomFilterMap,vaccSkipListMap,notVaccSkipListMap, virusesList, countryPopulationMap,countriesList, citizenMap, distinctCitizens);

    exit(EXIT_SUCCESS);
}
