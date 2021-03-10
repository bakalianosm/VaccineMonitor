


/* --------------------------------------------------------
 * ----------------- VACCINE MONITOR ----------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

#define K_FOR_BF 16
/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* -------- User-defined header files ------- */
#include "common.h"
#include "map.h"
#include "skipList.h"
#include "linkedList.h"
#include "bloomFilter.h"
#include "vaccineMonitor.h"
#include "generalFunctions.h"


int main(int argc, char *argv[]){
    
    if (argc != 5) exit(EXIT_FAILURE);
    printf("Hello from Vaccine Monitor\n");
    
    /* Arguments parsing */
    char *recordFile;
    int bloomSize;
    if( (strcmp(argv[1], "-c")==0) && (strcmp(argv[3], "-b")==0) ){
            recordFile = strdup(argv[2]);
            bloomSize = atoi(argv[4]);      
        }
    else{
        printf("Invalid command line sequence.Exiting...\n");
        exit(EXIT_FAILURE);
    }


    FILE* input_fp ;
    if (recordFile != NULL) input_fp = fopen(recordFile, "r");

    if (input_fp == NULL){
        perror("Error opening record file ");
    }
    
    /* Initialize the bloom fiter */
    BloomFilter bloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

    Map map = map_create(compare_records,record_destroy_key, record_destroy_value );
    map_set_hash_function(map,hash_int);

    /* string buffer for each line */
    char buffer[256];

    /* Values to be assigned */
    while( fgets(buffer,sizeof(buffer),input_fp) != NULL ){
        int parsedID = -1 ;
        char *parsedFirstName = NULL; 
        char *parsedLastName = NULL ;
        char *parsedCountry = NULL ;
        int parsedAge = -1 ;
        char *parsedVirusName = NULL ;
        char *parsedIsVaccinated = NULL ;
        char *parsedDateVaccinated = NULL ;

        /* this array represents an array of the values of input line */
        char *array[9];

        /* parse the values from the buffer and make an array of these */
        parseValues(buffer,array);

        /* assign the values of the array to the correct variables and validate them */
        assignValues(array,&parsedID, &parsedFirstName, &parsedLastName, &parsedCountry, &parsedAge, &parsedVirusName, &parsedIsVaccinated, &parsedDateVaccinated);
    
        /* create a new record */
        Record citizen = malloc(sizeof(*citizen));

        /* and initialize with the parsed values */
        citizen->ID = parsedID;
        citizen->firstName = strdup(parsedFirstName);
        citizen->lastName = strdup(parsedLastName);
        citizen->country = strdup(parsedCountry);
        citizen->age = parsedAge;
        citizen->virusName = parsedVirusName;
        citizen->isVaccinated = parsedIsVaccinated;
        citizen->dateVaccinated = parsedDateVaccinated;



    }


}