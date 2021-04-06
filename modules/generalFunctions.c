


/* --------------------------------------------------------
 * ---------------- GENERAL FUNCTIONS ---------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* -------- User-defined header files ------- */
#include "generalFunctions.h"
#include "bloomFilter.h"
#include "linkedList.h"
#include "skipList.h"
#include "dates.h"
#include "common.h"

#define K_FOR_BF 16
#define MAX_SKIPLIST_LEVEL 6

uint hash_vaccine(Pointer value){
    uint first = hash_int(((hashRec)value)->ID);
    uint last = hash_string(((hashRec)value)->virus);
	return first + last;
}

void printRed(const char* sentence){
    if(sentence != NULL){
        printf("\033[1;31m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}

void printGreen(const char* sentence){
    if(sentence != NULL){
        printf("\033[0;32m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}
int* create_int(int value){
    int *p = malloc(sizeof(int));
    *p = value ;
    return p;

}
int compare_keys(Pointer a, Pointer b){
    hashRec first = (hashRec)a;
    hashRec second = (hashRec)b;

    /* The main use of this function is to check when 2 given hashes are the same */
    /* And it's only when we have the same id and virus */
    if ( (*(int*)first->ID == *(int*)second->ID) && (strcmp(first->virus, second->virus) == 0) )
        return 0;
    else
        return 1;


}

int compare_IDs(Pointer a, Pointer b){
    return ((Record)a)->ID -((Record)b)->ID ;
}

void record_destroy_key(Pointer value){
    /* Free the struct container */
    free(((hashRec)value)->ID);

    /* Free the whole struct */
    free((hashRec)value);
}

void record_destroy_value(Pointer rec){

    /* Start and free each part of a Record struct */
    free(((Record)rec)->firstName);
    free(((Record)rec)->lastName);
    free(((Record)rec)->country);
    free(((Record)rec)->virusName);

    /* Not all records contain date because some are NOT VACCINATED */
    if( strcmp(((Record)rec)->isVaccinated, "YES") == 0){
        /* If is vaccinated, destroy it's date, too */
        free(((Record)rec)->dateVaccinated);
    }
    free(((Record)rec)->isVaccinated);
    free(((Record)rec));
}

int compare_viruses(Pointer a, Pointer b){
    return (strcmp(a,b));
}

int compare_countries(Pointer a, Pointer b){
    return (strcmp(a,b));
}
void destroy_virus(Pointer value){
    free(value);
}

void destroy_virus_bf(Pointer rec){
    bf_destroy((BloomFilter)rec);
}

void destroy_vacc_skip_list(Pointer value){
    SL_destroy((SkipList)value);
}

void parseValues(char buffer[], char* array[]){
    int i =0;
    
    array[i] = strtok(buffer, " ");
    while(array[i] !=NULL ){
        array[++i] = strtok(NULL, " ");
    }

}



ERR_CHK assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, char** dateVaccinated){
    ERR_CHK ERRNO = NO_ERROR;

    *ID = atoi(valuesArray[0]);

    *firstName = strdup(valuesArray[1]);
    *lastName = strdup(valuesArray[2]);
    *country = strdup(valuesArray[3]);
    *age = atoi(valuesArray[4]);
    *virusName = strdup(valuesArray[5]);
    *isVaccinated = strdup(valuesArray[6]);

    // if(strcmp(*isVaccinated, "NO\n")==0){
    //     *isVaccinated = strdup("NO");
    // }

    if (valuesArray[7] != NULL){
        *dateVaccinated = strdup(valuesArray[7]);
    }
    else {
        dateVaccinated = NULL;
    }

    /* -------------------------------- VALIDATION CHECK -----------------------------------------*/
    /*  printf("%d %s %s %s %d %s %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated, *dateVaccinated); */

    /* age check */
    if(! (*age > 0 && *age <= 120)) {
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        if( dateVaccinated != NULL) printf("%s\n", *dateVaccinated);
        ERRNO = AGE_ERR;
    }

    /* if vaccinated is no and provided a date */
    if( (strcmp(valuesArray[6],"NO") == 0) && valuesArray[7] != NULL)  {
       printf("ERROR IN RECORD %d %s %s %s %d %s %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName,*isVaccinated, *dateVaccinated);
       ERRNO = VACCINATED_DAY_ERR;
    }
    /* if is vaccinated and not given a date  */
    if( (strcmp(valuesArray[6],"YES") == 0) && ( valuesArray[7] == NULL ) ){
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        ERRNO = NOT_GIVEN_DATE_ERR;
    }

    if( (strcmp(valuesArray[6],"YES\n") == 0) && ( valuesArray[7] == NULL ) ){
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        ERRNO = NOT_GIVEN_DATE_ERR;
    }

    return ERRNO;
}

ERR_CHK assignValuesForIoption(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, char** dateVaccinated){
    ERR_CHK ERRNO = NO_ERROR;
    
    *ID = atoi(valuesArray[1]);

    *firstName = strdup(valuesArray[2]);
    *lastName = strdup(valuesArray[3]);
    *country = strdup(valuesArray[4]);
    *age = atoi(valuesArray[5]);
    *virusName = strdup(valuesArray[6]);
    *isVaccinated = strdup(valuesArray[7]);


    if (valuesArray[8] != NULL){
        *dateVaccinated = strdup(valuesArray[8]);
    }
    else {
        dateVaccinated = NULL;
    }

    /* -------------------------------- VALIDATION CHECK -----------------------------------------*/
    /*  printf("%d %s %s %s %d %s %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated, *dateVaccinated); */

    /* age check */
    if(! (*age > 0 && *age <= 120)) {
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        if( dateVaccinated != NULL) printf("%s\n", *dateVaccinated);
        ERRNO = AGE_ERR;
    }

    /* if vaccinated is no and provided a date */
    if( (strcmp(valuesArray[7],"NO") == 0) && valuesArray[8] != NULL)  {
       printf("ERROR IN RECORD %d %s %s %s %d %s %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName,*isVaccinated, *dateVaccinated);
       ERRNO = VACCINATED_DAY_ERR;
    }
    /* if is vaccinated and not given a date  */
    if( (strcmp(valuesArray[7],"YES") == 0) && ( valuesArray[8] == NULL )){
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        ERRNO = NOT_GIVEN_DATE_ERR;
    }
    
    return ERRNO;
}
hashRec initializeHashKey(int ID, char* virus){
    hashRec hashrec = malloc(sizeof(*hashrec));

    hashrec->ID = create_int(ID);
    hashrec->virus = virus;

    return hashrec;


}

Record initializeCitizen(int ID,   char *firstName, char *lastName, char *country, int age, char *virusName, char *isVaccinated, char* dateVaccinated){
    Record citizen = malloc(sizeof(*citizen));

    /* and initialize with the parsed values */
    citizen->ID = ID;
    citizen->firstName = strdup(firstName);
    citizen->lastName = strdup(lastName);
    citizen->country = strdup(country);
    citizen->age = age;
    citizen->virusName = strdup(virusName);

    citizen->isVaccinated = strdup(isVaccinated);

    if(dateVaccinated != NULL ){
        citizen->dateVaccinated = transformDate(dateVaccinated);
    }

    return citizen;
    
}

void printCitizen(Record citizen){
    if (strcmp(citizen->isVaccinated, "YES") == 0){
        printf("%d %s %s %s %d %s %s ", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age, citizen->virusName, citizen->isVaccinated);
        printDate(citizen->dateVaccinated);
    }
    else {
        printf("%d %s %s %s %d %s %s", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age, citizen->virusName, citizen->isVaccinated);
    }
}

int countArgs(char* str) {
    int cnt = 0;
    char* tok;
    char delim[2] = " ", temp[BUFFER_SIZE]; /* afto mporei na mi xreiazetai to buffer_size */
    strcpy(temp, str);
    tok = strtok(temp, delim);

    while( tok != NULL ) {
        tok = strtok(NULL, delim);
        cnt++;
    }

    return cnt;

}

int checkID(char str[]){

    int length = strlen(str);

    if (length > 4 ) return 0;
    for (int i = 0 ; i < length ; i++){
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}

void removeChar(char *str, char toRem){
    char *src, *dst;

    for(src = dst = str; *src !='\0' ; src++){
        *dst = *src;
        if (*dst != toRem) dst++;
    }
    *dst = '\0';
}


void freeAllocatedMemory(FILE* input_fp,char* recordFile,LinkedList virusesList, LinkedList countriesList, Map citizenMap,Map distinctCitizens, Map bloomFilterMap, Map vaccSkipListMap, Map notVaccSkipListMap, Map countryPopulationMap){
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

void updatePopulation(Map distinctCitizens,Map countryPopulationMap ,Record citizen){

    /* Check if the ID of the given citizen exists on the Map of Distinct citizens */
    MapNode toFind = map_find_node(distinctCitizens, &citizen->ID);
    if (toFind == NULL){
        /* And not exists  add it*/
        map_insert(distinctCitizens,&citizen->ID, citizen);

        /* Find the population of the citizen's country */
        MapNode m = MAP_EOF;
        m = map_find_node(countryPopulationMap, citizen->country);
        if(m != MAP_EOF){
            /* If we have already a value about the countrie's population, just increase it */
            int* population = map_node_value(countryPopulationMap, m);
            (*population)++;
        }
        else{
            /* If we do not have the population of the country, it's the first 
               time that we face a citizen with this country. Add the value ->(1) */
            map_insert(countryPopulationMap, citizen->country, create_int(1));
        }
    }
}

void updateVirusList(LinkedList virusesList, Record citizen){
    ListNode lnode = NULL;

    /* Check if the virus exists on the virusesList */
    lnode = LL_find_node(virusesList, citizen->virusName, compare_viruses);
    if (lnode == NULL){
        /* And if does not exist, add the virus */
        LL_insert_after(virusesList, NULL, citizen->virusName);
    }
}

void updateCountyList(LinkedList countriesList, Record citizen){
    ListNode lnode = NULL;

    /* Check if the country exists on the virusesList */
    lnode = LL_find_node(countriesList, citizen->country, compare_countries);
    if (lnode == NULL){
         /* And if does not exist, add the country */
        LL_insert_after(countriesList, NULL, citizen->country);
    }
}




void insertCitizenInBloomFilter(int bloomSize,Map bloomFilterMap, Record citizen){
    MapNode m = MAP_EOF;
    BloomFilter virusBloomFilter;

    /* Transform the id again to string  */
    char str[10];
    sprintf(str, "%d", citizen->ID);
    unsigned char* idString = (unsigned char*)strdup(str);

    /* Check if exists a Bloom Filter for the citizen's virus */
    m = map_find_node(bloomFilterMap,citizen->virusName);
    if (m != MAP_EOF){
        /* And if exists */
        /* Get the Bloom Filter for this virus */
        virusBloomFilter = (BloomFilter)map_node_value(bloomFilterMap, m);
        if (virusBloomFilter == NULL) { printf("Error!\n") ; exit(EXIT_FAILURE); }

        /* Insert the given ID to the Bloom Filter for the virus */
        bf_insert(virusBloomFilter, idString);

    }
    else {
        /* If does not exist */


        /* Create a bloom filter for this virus */
        virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

        bf_insert(virusBloomFilter, idString);
        
        /* Insert the new Bloom Filter in the Map of Bloom Filters */
        map_insert(bloomFilterMap, citizen->virusName, virusBloomFilter);

    }

    free(idString);
}

void insertCitizenInVaccSkipList(Map vaccSkipListMap, Record citizen){
    
    SkipList virusSkipList;
    MapNode m = map_find_node(vaccSkipListMap, citizen->virusName);

    /* Check if exist  [VACCINATED] Skip List with this virus */
    if (m != MAP_EOF){
        /* If exists insert citizen in this skiplist*/

        /* Get the Skip List for this virus */
        virusSkipList = (SkipList)map_node_value(vaccSkipListMap,m);
        if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

        /* Insert the given citizen in the Skip List */
        SL_insert(virusSkipList, &citizen->ID,citizen, compare_IDs);

    }
    else{
        /* if dont exists create skiplist, insert record */

        /* Create a skiplist for this virus */
        virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);

        /* Insert the given citizen in the Skip List */
        SL_insert(virusSkipList, &citizen->ID, citizen, compare_IDs);

        /* Insert the new [VACCINATED] SkipList int the Map of Vaccinated Skip Lists */
        map_insert(vaccSkipListMap, citizen->virusName ,virusSkipList);

    }

}

void insertCitizenInNotVaccSkipList(Map notVaccSkipListMap, Record citizen){

    SkipList virusSkipList;
    MapNode m = map_find_node(notVaccSkipListMap, citizen->virusName);


    /* Check if exist  [NOT ACCINATED] Skip List with this virus */
    if (m != MAP_EOF){
        /* if exists insert citizen in this skiplist*/

        /* Get the Skip List for this virus */
        virusSkipList = (SkipList)map_node_value(notVaccSkipListMap,m);
        if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

        /* Insert the given citizen in the Skip List */
        SL_insert(virusSkipList, &citizen->ID ,citizen, compare_IDs);

    }
    else{
        /* if dont exists create skiplist, insert record */

        /* Create a skiplist for this virus */
        virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);

        /* Insert the given citizen in the Skip List */
        SL_insert(virusSkipList, &citizen->ID ,citizen, compare_IDs);
        
        /* Insert the new [NOT VACCINATED] SkipList int the Map of Vaccinated Skip Lists */
        map_insert(notVaccSkipListMap, citizen->virusName,virusSkipList);

    }


}

void destroyAssistantValues(char* parsedFirstName, char* parsedLastName, char* parsedCountry, char* parsedVirusName, char* parsedIsVaccinated, char* parsedDateVaccinated){
    if(parsedFirstName !=NULL) free(parsedFirstName);
    if(parsedLastName !=NULL) free(parsedLastName);
    if(parsedCountry !=NULL) free(parsedCountry);
    if(parsedVirusName !=NULL) free(parsedVirusName);
    if(parsedIsVaccinated !=NULL) free(parsedIsVaccinated);
    if(parsedDateVaccinated!=NULL) free(parsedDateVaccinated); 
}

void initialiazeMonitorDataStructures(LinkedList* virusesList, LinkedList* countriesList, Map* distinctCitizens, Map* citizenMap, Map* bloomFilterMap, Map* vaccSkipListMap, Map* notVaccSkipListMap, Map* countryPopulationMap){
    /* Initialize a list that will contain only the viruses */
    *virusesList =  LL_create(NULL);

    /* Initialize a list that will contain only the countries */
    *countriesList =  LL_create(NULL);


    /* a map that holds distinct records */
    *distinctCitizens = map_create(compare_IDs, NULL, NULL);
    map_set_hash_function(*distinctCitizens, hash_int);

    /* Initialize the Citizen Map */
    *citizenMap = map_create(compare_keys,record_destroy_key, record_destroy_value );
    map_set_hash_function(*citizenMap,hash_vaccine);

    /* Initialize the Bloom Filter Map */
    *bloomFilterMap =  map_create(compare_viruses, NULL, destroy_virus_bf);
    map_set_hash_function(*bloomFilterMap,hash_string);


    /* Initialize the VACCINATED Skip List Map */
    *vaccSkipListMap = map_create(compare_viruses, NULL, destroy_vacc_skip_list);
    map_set_hash_function(*vaccSkipListMap, hash_string);


    /* Initialize the NOT VACCINATED Skip List Map */
    *notVaccSkipListMap = map_create(compare_viruses, NULL, destroy_vacc_skip_list);
    map_set_hash_function(*notVaccSkipListMap, hash_string);


    /* A map that hold for each country its population */
    *countryPopulationMap = map_create(compare_countries, NULL, free);
    map_set_hash_function(*countryPopulationMap, hash_string);
}