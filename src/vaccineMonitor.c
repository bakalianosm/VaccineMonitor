


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
#include "bloomFilter.h"

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


    /* Initialize a list that will contain only the viruses */
    LinkedList virusesList =  LL_create(NULL);

    /* Initialize a list that will contain only the countries */
    LinkedList countriesList =  LL_create(NULL);


    /* a map that holds distinct records */
    Map distinctCitizens = map_create(compare_IDs, NULL, NULL);
    map_set_hash_function(distinctCitizens, hash_int);

    /* Initialize the Citizen Map */
    Map citizenMap = map_create(compare_keys,record_destroy_key, record_destroy_value );
    map_set_hash_function(citizenMap,hash_vaccine);

    /* Initialize the Bloom Filter Map */
    Map bloomFilterMap =  map_create(compare_viruses, NULL, destroy_virus_bf);
    map_set_hash_function(bloomFilterMap,hash_string);


    /* Initialize the VACCINATED Skip List Map */
    Map vaccSkipListMap = map_create(compare_viruses, NULL, destroy_vacc_skip_list);
    map_set_hash_function(vaccSkipListMap, hash_string);


    /* Initialize the NOT VACCINATED Skip List Map */
    Map notVaccSkipListMap = map_create(compare_viruses, NULL, destroy_vacc_skip_list);
    map_set_hash_function(notVaccSkipListMap, hash_string);


    /* A map that hold for each country its population */
    Map countryPopulationMap = map_create(compare_countries, NULL, free);
    map_set_hash_function(countryPopulationMap, hash_string);


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
        if(error != NO_ERROR){
            if(parsedFirstName !=NULL) free(parsedFirstName);
            if(parsedLastName !=NULL) free(parsedLastName);
            if(parsedCountry !=NULL) free(parsedCountry);
            if(parsedVirusName !=NULL) free(parsedVirusName);
            if(parsedIsVaccinated !=NULL) free(parsedIsVaccinated);
            if(parsedDateVaccinated!=NULL) free(parsedDateVaccinated);
            continue;
        } 

        /* create a new record */
        Record citizen = initializeCitizen(parsedID, parsedFirstName, parsedLastName, parsedCountry, parsedAge, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
        /* the hash will be a combination of ID and virus */
        hashRec hashKey = initializeHashKey(citizen->ID, citizen->virusName);

        map_insert(citizenMap, hashKey, citizen);

        /* ----------------- COUNTRY POPULATION INSERT --------------------- */

        /* increment the country population only if the record does not exists for
        any other virus list */
        MapNode toFind = map_find_node(distinctCitizens, &citizen->ID);
        if (toFind == NULL){
            map_insert(distinctCitizens,&citizen->ID, citizen);

            MapNode m = MAP_EOF;
            m = map_find_node(countryPopulationMap, citizen->country);
            if(m != MAP_EOF){
                int* population = map_node_value(countryPopulationMap, m);
                (*population)++;
            }
            else{
                map_insert(countryPopulationMap, citizen->country, create_int(1));
            }
        }

        ListNode lnode = NULL;

         /* insert virus on the list with the viruses if not extists */
        lnode = LL_find_node(virusesList, citizen->virusName, compare_viruses);
        if (lnode == NULL){
            LL_insert_after(virusesList, NULL, citizen->virusName);
        }

         /* insert virus on the list with the countries if not extists */
        lnode = LL_find_node(countriesList, citizen->country, compare_countries);
        if (lnode == NULL){
            LL_insert_after(countriesList, NULL, citizen->country);
        }

        
        if(strcmp(citizen->isVaccinated,"YES")==0 || strcmp(citizen->isVaccinated,"YES\n")==0 ){

            /* ----------------- VACCINATED RECORD INSERT ON THE BLOOM FILTER FOR THIS VIRUS --------------------- */

            printf("%d IS VACCINATED FOR %s\n",citizen->ID,citizen->virusName);


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
                // printf("%s inserted in BF for %s\n",idString, citizen->virusName);

            }
            else {

                // printf("Dont exists this virus %s. Going to insert it\n",citizen->virusName);

                /* create a bloom filter for this virus */
                virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

                bf_insert(virusBloomFilter, idString);
                // printf("Created bf for %s\n",citizen->virusName);
                // printf("%s inserted in BF for %s\n",idString, citizen->virusName);
                /* insert the bloomFilter in the map */

                map_insert(bloomFilterMap, citizen->virusName, virusBloomFilter);

            }

            free(idString);

            /* ----------------- VACCINATED RECORD INSERT ON [VACCINATED] SKIP LIST FOR THIS VIRUS --------------------- */
            SkipList virusSkipList;
            m = map_find_node(vaccSkipListMap, citizen->virusName);


            /* check if exist skip list with this virus */
            if (m != MAP_EOF){
            /* if exists insert citizen in this skiplist*/
                virusSkipList = (SkipList)map_node_value(vaccSkipListMap,m);
                if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

                /* compare values is compare ID */
                SL_insert(virusSkipList, &citizen->ID,citizen, compare_IDs);
                // printf("%d inserted in %s VACCINATED skiplist\n", citizen->ID, citizen->virusName);
            }
            else{
            /* if dont exists create skiplist, insert record */

                /* create a skiplist for this virus */
                virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);
                /* compare values is compare ID */

                SL_insert(virusSkipList, &citizen->ID, citizen, compare_IDs);
                // printf("%d inserted in NEW %s VACCINATED skiplist\n", citizen->ID, citizen->virusName);

                map_insert(vaccSkipListMap, citizen->virusName ,virusSkipList);

            }

           

        }
        else if(strcmp(citizen->isVaccinated,"NO\n")==0 || strcmp(citizen->isVaccinated,"NO")==0){
            /* if the citizen is NOT VACCINATED */

            /* -------------- NOT VACCINATED RECORD INSERT ON THE BLOOM FILTER FOR THIS VIRUS --------------------- */

            SkipList virusSkipList;
            MapNode m = map_find_node(notVaccSkipListMap, citizen->virusName);


            /* check if exist skip list with this virus */
            if (m != MAP_EOF){
            /* if exists insert citizen in this skiplist*/
                virusSkipList = (SkipList)map_node_value(notVaccSkipListMap,m);
                if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

                /* compare values is compare ID */
                SL_insert(virusSkipList, &citizen->ID ,citizen, compare_IDs);
                // printf("%d inserted in %s VACCINATED skiplist\n", citizen->ID, citizen->virusName);
            }
            else{
            /* if dont exists create skiplist, insert record */

                /* create a skiplist for this virus */
                virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);
                /* compare values is compare ID */

                SL_insert(virusSkipList, &citizen->ID ,citizen, compare_IDs);
                // printf("%d inserted in NEW %s **NOT** VACCINATED skiplist\n", citizen->ID, citizen->virusName);

                map_insert(notVaccSkipListMap, citizen->virusName,virusSkipList);

            }


             
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

    /* read user's input from keyboard */
    USR_INPT input;
    while ( ( input = readUserInput(bloomSize, bloomFilterMap,vaccSkipListMap,notVaccSkipListMap, virusesList, countryPopulationMap,countriesList)) != USR_EXIT){
        switch (input){
            case INPT_1:
                printGreen("vaccineStatusBloom\n");
                break;

            case INPT_2:
                printGreen("vaccineStatus\n");
                break;

            case INPT_3:
                printGreen("populationStatus\n");
                break;

            case INPT_4:
                printGreen("popStatusbyAge\n");
                break;

            case INPT_5:
                printGreen("insertCitizenRecord\n");
                break;

            case INPT_6:
                printGreen("vaccinateNow\n");
                break;

            case INPT_7:
                printGreen("list-nonVaccinated-Persons\n");
                break;

            case INVALID_INPT:
                printRed("Please give a valid instruction\n");
                break;
            case ARG_ERR:
                printRed("Please give valid arguments\n");

        }
    }


    printf("Record inserted : %d\n",map_size(citizenMap));
    printf("Distinct record inserted : %d\n",map_size(distinctCitizens));
    printf("Bloomfilters created for VACCINATED people : %d\n",map_size(bloomFilterMap));
    printf("Skiplists created for VACCINATED people : %d\n",map_size(vaccSkipListMap));
    printf("Skiplists created for NOT VACCINATED people : %d\n",map_size(notVaccSkipListMap));



    fclose(input_fp);
    free(recordFile);
    LL_destroy(virusesList);
    LL_destroy(countriesList);
    map_destroy(citizenMap);
    map_destroy(distinctCitizens);
    map_destroy(bloomFilterMap);
    map_destroy(vaccSkipListMap);
    map_destroy(notVaccSkipListMap);
    map_destroy(countryPopulationMap);

}
