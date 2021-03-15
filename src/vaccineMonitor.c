


/* --------------------------------------------------------
 * ----------------- VACCINE MONITOR ----------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ---------------- Create Constants -------- */
#define K_FOR_BF 16


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
#include "bloomFilter.h"
#include "monitorChoices.h"
#include "vaccineMonitor.h"
#include "generalFunctions.h"


int main(int argc, char *argv[]){
    

    /* ---------------------------- READ COMMAND LINE ARGUMENTS ---------------------------- */
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

    /* ------------------------------ OPEN INPUT FILE ---------------------------------------- */

    /* Define a file pointer */
    FILE* input_fp ;

    /* And open the file */
    if (recordFile != NULL) input_fp = fopen(recordFile, "r");
    /* Check */
    if (input_fp == NULL){
        perror("Error opening record file ");
    }
    

    /* --------------------------------- DATA STRUCTURES INITIALIZATION ----------------------------- */
    

    /* Initialize the Citizen Map */
    Map citizenMap = map_create(compare_keys,record_destroy_key, record_destroy_value );
    map_set_hash_function(citizenMap,hash_vaccine);

    /* Initialize the Bloom Filter Map */
    Map bloomFilterMap =  map_create(compare_viruses, destroy_virus, destroy_virus_bf);
    map_set_hash_function(bloomFilterMap,hash_string);

    /* string buffer for each line */
    char buffer[BUFFER_SIZE];

    /* ---------------------------------- DATA PARSING ------------------------------------------------ */
    char *array[9] = {NULL, NULL, NULL , NULL , NULL, NULL , NULL, NULL, NULL} ;
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
        

        /* parse the values from the buffer and make an array of these */
        parseValues(buffer,array);

        /* assign the values of the array to the correct variables and validate them */
        ERR_CHK error = assignValues(array,&parsedID, &parsedFirstName, &parsedLastName, &parsedCountry, &parsedAge, &parsedVirusName, &parsedIsVaccinated, &parsedDateVaccinated);
        
        /* if the above function return an error code , we have to ignore this record */
        if(error != NO_ERROR) continue;
        
        /* create a new record */
        Record citizen = initializeCitizen(parsedID, parsedFirstName, parsedLastName, parsedCountry, parsedAge, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
        /* the hash will be a combination of ID and virus */
        hashRec hashKey = initializeHashKey(citizen->ID, citizen->virusName);

        map_insert(citizenMap, hashKey, citizen);

        if(strcmp(citizen->isVaccinated,"YES")==0){
            MapNode m = MAP_EOF;
            BloomFilter virusBloomFilter;

            /* itransform the id again to string  */
            char str[10];
            sprintf(str, "%d", citizen->ID);
            unsigned char* idString = (unsigned char*)strdup(str);
            m = map_find_node(bloomFilterMap,citizen->virusName);
            if (m != MAP_EOF){
                // printf("Exists this virus\n");
                virusBloomFilter = (BloomFilter)map_node_value(bloomFilterMap, m);
                if (virusBloomFilter == NULL) { printf("Error!\n") ; exit(EXIT_FAILURE); }
                bf_insert(virusBloomFilter, idString);
                printf("%s inserted in BF for %s\n",idString, citizen->virusName);
            }
            else {

                // printf("Dont exists this virus %s. Going to insert it\n",citizen->virusName);
                
                /* create a bloom filter for this node */
                virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

                bf_insert(virusBloomFilter, idString);
                printf("%s inserted in BF for %s\n",idString, citizen->virusName);
                /* insert the bloomFilter in the map */
                map_insert(bloomFilterMap, strdup(citizen->virusName), virusBloomFilter);
                
            }

            free(idString);

        }
        
        /* print citizen's fields to be sure */
        // printCitizen(citizen);

        if(parsedFirstName !=NULL) free(parsedFirstName);
        if(parsedLastName !=NULL) free(parsedLastName);
        if(parsedCountry !=NULL) free(parsedCountry);
        if(parsedVirusName !=NULL) free(parsedVirusName);
        if(parsedIsVaccinated !=NULL) free(parsedIsVaccinated);
        if(parsedDateVaccinated!=NULL) free(parsedDateVaccinated);

    }

    // /* read user's input from keyboard */
    // USR_INPT input;
    // while ( ( input = readUserInput()) != USR_EXIT){
    //     switch (input){
    //         case INPT_1:
    //             printGreen("vaccineStatusBloom\n");
    //             break;
            
    //         case INPT_2:
    //             printGreen("vaccineStatus\n");
    //             break;
                
    //         case INPT_3:
    //             printGreen("populationStatus\n");
    //             break;
    
    //         case INPT_4:
    //             printGreen("popStatusbyAge\n");
    //             break;

    //         case INPT_5:
    //             printGreen("insertCitizenRecord\n");
    //             break;  

    //         case INPT_6:
    //             printGreen("vaccinateNow\n");
    //             break;
                    
    //         case INPT_7:
    //             printGreen("list-nonVaccinated-Persons\n");
    //             break;
                    
    //         case INVALID_INPT:
    //             printRed("Please give a valid instruction\n");
    //             break;
    //         case ARG_ERR:
    //             printRed("Please give valid arguments\n");
                    
    //     }
    // }
    printf("Record inserted : %d\n",map_size(citizenMap));
    printf("Bloomfilters created : %d\n",map_size(bloomFilterMap));
    

    // for (MapNode node = map_first(citizenMap); node != MAP_EOF; node = map_next(citizenMap, node)) {
    //     printf("mpike");
    //     hashRec key = map_node_key(citizenMap,node);
    //     Record value = map_node_value(citizenMap,node);
    //     printf("edo");
    //     printCitizen(value);
    // }

    fclose(input_fp);
    free(recordFile);
    map_destroy(citizenMap);
    map_destroy(bloomFilterMap);

}