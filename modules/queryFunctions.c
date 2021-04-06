


/* -------------------------------------------------------
 * -------------- QUERY FUNCTIONS ------------------------
 * --------------- IMPLEMENTATION ------------------------
 * -------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* -------- User-defined header files ------- */
#include "queryFunctions.h"
#include "generalFunctions.h"
#include "bloomFilter.h"
#include "linkedList.h"
#include "skipList.h"
#include "dates.h"
#include "common.h"


void vaccineStatusBloom(char str[], char* arr[], Map bloomFilterMap){
    
    /* Put the arguments user inserted into a new array that will contain them */
    parseValues(str, arr);

    char* virus = NULL;
    unsigned char* IDstring = NULL;
    /* Check if the given ID is correct */
    if (!checkID(arr[1])){
        printRed("vaccineStatusBloom : ERROR! Please give a valid id [e.x. xxxx , x is NUMBER]\n"); 
        /* invalid id. either alpharethmetic either length >4 */
        return ;
    } 

    /* Virus is the second argument and we have to remove the endline character */
    virus = strdup(arr[2]);
    removeChar(virus,'\n');

    /* Create a string with the given ID for easy search in Bloom Filter */
    IDstring = (unsigned char*)strdup(arr[1]);

    /* Search for a Bloom Filter for this [VIRUS] */
    MapNode m = MAP_EOF;    
    m = map_find_node(bloomFilterMap, virus);

    /* If exists a Bloom Filter for this [VIRUS] */
    if(m != MAP_EOF){

        /* Search for the given ID */
        BloomFilter virusBF = (BloomFilter)map_node_value(bloomFilterMap, m);
        if(bf_search(virusBF, IDstring)){
            printf("MAYBE\n");
        }
        else{
            printf("NOT VACCINATED\n");
        }
    }
    else {
        /* If not exists a Bloom Filter print the corresponding output */
        printRed("Sorry no BF for this virus. \n");
    }


    /* Free used pointers */
    if(virus != NULL) free(virus);
    if(IDstring != NULL) free(IDstring);

    /* Everything went good, continue */
    return ;

}

void vaccineStatus(int numOfArguments,char str[], char* arr[], Map vaccSkipListMap,Map notVaccSkipListMap, LinkedList virusesList){


    if(numOfArguments ==2){

        int searchID;
        char* virus = NULL;
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);

        /* Check if the given ID is correct */
        if (!checkID(arr[1])){
        printRed("vaccineStatus: ERROR! Please give a valid id [e.x. xxxx , x is NUMBER]\n"); 
        /* invalid id. either alpharethmetic either length >4 */
        return ;
    } 

        /* Virus is the second argument and we have to remove the endline character */
        virus = strdup(arr[2]);
        removeChar(virus,'\n');

       

        /* Create an int* with the ID for searching in Skip List */
        searchID = atoi(arr[1]);

        /* Search for a VACCINATED Skip List for this [VIRUS] */
        Record citizen = NULL;
        citizen = searchForCitizenInVaccSkipList(&searchID, vaccSkipListMap, virus);

        if (citizen != NULL){
            if(citizen->dateVaccinated != NULL){
                printf("VACCINATED ON ");
                printDate(citizen->dateVaccinated);

            }
        }
        else{
            printf("NOT VACCINATED\n");
        }
        
        if(virus != NULL ) free(virus);

        return;
        

    }
    else if (numOfArguments ==1){

        int searchID ;
        char* virus = NULL;

        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);      
        removeChar(arr[1],'\n');
        searchID = atoi(arr[1]);

        /* Check if the given ID is correct */
        if (searchID > 9999 || searchID < 0){
        printRed("vaccineStatus : ERROR! Please give a valid id [e.x. xxxx , x is NUMBER]\n"); 
        /* invalid id. either alpharethmetic either length >4 */
        return ;
    } 

        for (ListNode node = LL_first(virusesList) ; node != NULL ; node = LL_next(node)  ){
            /* Get the virus*/
            virus = (char*)LL_node_val(node);

            /* Search in VACCINATED Skip List for this virus */
            Record citizen = NULL;
            citizen = searchForCitizenInVaccSkipList(&searchID,vaccSkipListMap, virus);
            if(citizen != NULL){            
                printf("%s YES ", virus);
                printDate(citizen->dateVaccinated);
            }
            
            
            
            /* Search in NOT VACCINATED Skip List for this virus */
            citizen = searchForCitizenInNotVaccSkipList(&searchID, notVaccSkipListMap, virus);
            if (citizen != NULL){
                    printf("%s NO\n", virus);
            }
        }
        return ;
    }
}

Record searchForCitizenInVaccSkipList(int* searchID, Map vaccSkipListMap,char* virus){
    
    MapNode m = MAP_EOF;
    m = map_find_node(vaccSkipListMap, virus );

    /* If exists a VACCINATED Skip List for this [VIRUS] */
    if(m != MAP_EOF){

        /* Search for the given ID */
        skipListNode sl = NULL;
        SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap,m);
        sl = SL_find_node(virusSL,searchID,compare_IDs);
        if(sl != NULL){

            Record vaccinatedCitizen = NULL;
            /* If citizen with the given ID is into the Vaccinated Skip List, is VACCINATED */
            if (SL_node_val(sl) != NULL){
                vaccinatedCitizen = SL_node_val(sl);
            }

            /* Return it */
            if (vaccinatedCitizen != NULL) return vaccinatedCitizen;

        }
        else{
            return NULL;
        }
    }
    return NULL;
}

Record searchForCitizenInNotVaccSkipList(int* searchID, Map notVaccSkipListMap, char* virus){
    
    MapNode m = MAP_EOF;
    m = map_find_node(notVaccSkipListMap, virus);


    /* If exists a NOT VACCINATED Skip List for this [VIRUS] */
    if(m != MAP_EOF){

        /* Search for the given ID */
        skipListNode sl = NULL;
        SkipList virusSL = (SkipList)map_node_value(notVaccSkipListMap,m);
        sl = SL_find_node(virusSL,searchID,compare_IDs);
        if(sl != NULL){
            Record notVaccinatedCitizen = NULL;
            /* If citizen with the given ID is into the Vaccinated Skip List, is VACCINATED */
            if (SL_node_val(sl) != NULL){
                notVaccinatedCitizen = SL_node_val(sl);
            }

            /* Return it */
            if (notVaccinatedCitizen != NULL) return notVaccinatedCitizen;
        }
        else{
            return NULL;
        }
    }
    return NULL;
}

void populationStatus(int numOfArguments, char str[], char* arr[], Map vaccSkipListMap, Map notVaccSkipListMap, Map countryPopulationMap, LinkedList countriesList){

    if (numOfArguments == 3){
        
        char *country = NULL, *virus = NULL, *dateFrom = NULL, *dateTo = NULL;
        Date from = NULL, to = NULL; 
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);


        virus = strdup(arr[1]);
        dateFrom = strdup(arr[2]);
        dateTo = strdup(arr[3]);
        from = transformDate(dateFrom);
        to = transformDate(dateTo);

        /* check for valid dates */
        if (compareDates(from,to) > 0) {
            printRed("populationStatus : Error! Invalid dates\n");
            if(virus != NULL) free(virus);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            if(from != NULL) free(from);
            if(to != NULL) free(to);
            return;
        }

        for(ListNode node = LL_first(countriesList) ; node != NULL ; node = LL_next(node)){
            country = LL_node_val(node);
            findCountryPercentage(country, virus, from, to,vaccSkipListMap, countryPopulationMap);
        }

        
        if(virus != NULL) free(virus);
        if(dateFrom != NULL) free(dateFrom);
        if(dateTo != NULL) free(dateTo);
        if(from != NULL) free(from);
        if(to != NULL) free(to);

    }
    else if (numOfArguments == 4){
        char *country = NULL, *virus = NULL, *dateFrom = NULL, *dateTo = NULL;
        Date from = NULL, to = NULL; 

        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);

        country = strdup(arr[1]);
        virus = strdup(arr[2]);
        dateFrom = strdup(arr[3]);
        dateTo = strdup(arr[4]);
        from = transformDate(dateFrom);
        to = transformDate(dateTo);

        if (compareDates(from,to) > 0) {
            printRed("populationStatus : Error! Invalid dates\n");
            if(country != NULL) free(country);
            if(virus != NULL) free(virus);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            if(from != NULL) free(from);
            if(to != NULL) free(to);
            return;
        }
        
        findCountryPercentage(country, virus, from, to,vaccSkipListMap, countryPopulationMap);

        if(country != NULL) free(country);
        if(virus != NULL) free(virus);
        if(dateFrom != NULL) free(dateFrom);
        if(dateTo != NULL) free(dateTo);
        if(from != NULL) free(from);
        if(to != NULL) free(to);
    }
    return ;
}

void findCountryPercentage(char* country, char* virus, Date from,  Date to, Map vaccSkipListMap, Map countryPopulationMap){
    MapNode m = MAP_EOF;
    m = map_find_node(countryPopulationMap, country);
    if(m != MAP_EOF){
        int* pop = map_node_value(countryPopulationMap, m);
        /* Country's population */
        int population = *pop;
        
        /* Vaccinated people on country */
        int countryVaccinatedPeople = 0;
        
        MapNode m1 = map_find_node(vaccSkipListMap, virus);
        
        if(m1 != MAP_EOF){
            skipListNode sl =  NULL;
            SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap,m1);
            for(sl = SL_first(virusSL) ; sl != NULL ; sl = SL_next(sl)){
                Record citizen = SL_node_val(sl);
                if(compare_countries(country,citizen->country)==0){
                    if( (compareDates(from, citizen->dateVaccinated) <= 0) && (compareDates(to, citizen->dateVaccinated) >=0)){
                        countryVaccinatedPeople++;
                    }

                }
            }

        }

        
        float percentage = (float)countryVaccinatedPeople/population * 100;
        printf("%s %d %.2f %% \n",country, countryVaccinatedPeople,percentage );

    }

}

void popStatusByAge(int numOfArguments, char str[], char* arr[], Map vaccSkipListMap, Map notVaccSkipListMap, Map countryPopulationMap, LinkedList countriesList){
    if (numOfArguments == 3){
        
        char *country = NULL, *virus = NULL, *dateFrom = NULL, *dateTo = NULL;
        Date from = NULL, to = NULL; 
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);


        virus = strdup(arr[1]);
        dateFrom = strdup(arr[2]);
        dateTo = strdup(arr[3]);
        from = transformDate(dateFrom);
        to = transformDate(dateTo);

        /* Check for valid dates */
        if (compareDates(from,to) > 0) {
            printRed("popStatusByAge : Error! Invalid dates\n");
            if(virus != NULL) free(virus);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            if(from != NULL) free(from);
            if(to != NULL) free(to);
            return;
        }

        for(ListNode node = LL_first(countriesList) ; node != NULL ; node = LL_next(node)){
            int ageArr[4] = {0, 0, 0, 0};
            country = LL_node_val(node);
            findCountryPercentageByAge(country, virus, from, to, ageArr, vaccSkipListMap, countryPopulationMap);
        }

        if(virus != NULL) free(virus);
        if(dateFrom != NULL) free(dateFrom);
        if(dateTo != NULL) free(dateTo);
        if(from != NULL) free(from);
        if(to != NULL) free(to);
    }

    else if (numOfArguments == 4){

        char *country = NULL, *virus = NULL, *dateFrom = NULL, *dateTo = NULL;
        Date from = NULL, to = NULL; 
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);

        country = strdup(arr[1]);
        virus = strdup(arr[2]);
        dateFrom = strdup(arr[3]);
        dateTo = strdup(arr[4]);
        from = transformDate(dateFrom);
        to = transformDate(dateTo);

        if (compareDates(from,to) > 0) {
            printRed("popStatusByAge : Error! Invalid dates\n");
            if(country != NULL) free(country);
            if(virus != NULL) free(virus);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            if(from != NULL) free(from);
            if(to != NULL) free(to);
            return ;
        }

        int ageArr[4] = {0, 0, 0, 0};
        findCountryPercentageByAge(country, virus, from, to, ageArr, vaccSkipListMap, countryPopulationMap);

        if(country != NULL) free(country);
        if(virus != NULL) free(virus);
        if(dateFrom != NULL) free(dateFrom);
        if(dateTo != NULL) free(dateTo);
        if(from != NULL) free(from);
        if(to != NULL) free(to);
    }

    return ;
}

void findCountryPercentageByAge(char* country, char* virus, Date from,  Date to, int ageArr[], Map vaccSkipListMap, Map countryPopulationMap){
    printf("%s\n",country);
    MapNode m = MAP_EOF;
    m = map_find_node(countryPopulationMap, country);
    if(m != MAP_EOF){
        int* pop = map_node_value(countryPopulationMap, m);
        /* Country's population */
        int population = *pop;
        
        /* Vacinated people on coyntry */
        // int countryVaccinatedPeople = 0;
        
        MapNode m1 = map_find_node(vaccSkipListMap, virus);
        
        if(m1 != MAP_EOF){
            skipListNode sl =  NULL;
            SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap,m1);
            for(sl = SL_first(virusSL) ; sl != NULL ; sl = SL_next(sl)){
                Record citizen = SL_node_val(sl);
                if(compare_countries(country,citizen->country)==0){
                    if( (compareDates(from, citizen->dateVaccinated) <= 0) && (compareDates(to, citizen->dateVaccinated) >=0)){
                
                        if(citizen->age >= 0 && citizen->age <= 20){
                            ageArr[0]++;
                        }
                        else if(citizen->age > 20 && citizen->age <= 40){
                            ageArr[1]++;

                        }
                        else if(citizen->age > 40 && citizen->age <= 60){
                            ageArr[2]++;
                        }
                        else{
                            ageArr[3]++;
                        }
                    }
                }
            }
        }
        
        else{
            printRed("Error. No vaccinated records for this country and virus!\n");
            return;
        }

        float percentage1 = (float)ageArr[0]/population * 100;
        printf("0-20 %d %.2f%%\n", ageArr[0],percentage1 );

        float percentage2 = (float)ageArr[1]/population * 100;
        printf("20-40 %d %.2f%%\n", ageArr[1],percentage2 );

        float percentage3 = (float)ageArr[2]/population * 100;
        printf("40-60 %d %.2f%%\n", ageArr[2],percentage3 );

        float percentage4 = (float)ageArr[3]/population * 100;
        printf("60+ %d %.2f%%\n", ageArr[3],percentage4 );
    }
    printf("\n");
}

void insertCitizenRecord(int numOfArguments,int bloomSize, char str[], char* arr[], Map citizenMap, Map vaccSkipListMap, Map notVaccSkipListMap,Map distinctCitizens, Map bloomFilterMap, Map countryPopulationMap, LinkedList countriesList, LinkedList virusesList ){
    if (numOfArguments == 7 || numOfArguments ==8){
        int parsedID = -1, parsedAge = -1  ;
        char *parsedFirstName = NULL, *parsedLastName = NULL, *parsedCountry = NULL, *parsedVirusName = NULL, *parsedIsVaccinated = NULL, *parsedDateVaccinated = NULL ; 
        
        parseValues(str,arr);
        
        ERR_CHK error = assignValuesForIoption(arr,&parsedID, &parsedFirstName, &parsedLastName, &parsedCountry, &parsedAge, &parsedVirusName, &parsedIsVaccinated, &parsedDateVaccinated);

        if(error != NO_ERROR){
            destroyAssistantValues(parsedFirstName, parsedLastName, parsedCountry, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
            printf("insertCitizenRecord : ERROR data for insertion\n");
            return ;
        } 

        /* Create a new record */

        Record newCitizen = initializeCitizen(parsedID, parsedFirstName, parsedLastName, parsedCountry, parsedAge, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
        hashRec hashKey = initializeHashKey(newCitizen->ID, newCitizen->virusName);
        // printCitizen(newCitizen);

        /* Check if exists the same record, with ID and VIRUS in the citizenMap */
        MapNode toSearch = map_find_node(citizenMap, hashKey);

        
        if (toSearch != NULL){
            /* If exists */
            // printf("Exists a record with this ID and VIRUS\n");

            Record citizenOnMap = (Record)map_node_value(citizenMap, toSearch);
            
            /* If the record is vaccinated print error */
            if (strcmp(citizenOnMap->isVaccinated, "YES") == 0){
                printf("ERROR: CITIZEN %d ALREADY VACCINATED ON ", citizenOnMap->ID);
                printDate(citizenOnMap->dateVaccinated);
                record_destroy_key(hashKey);
                record_destroy_value(newCitizen);
            }
            else {

                if(strcmp(newCitizen->isVaccinated, "YES") == 0 ){
                    
                    /* Remove the old citizen from the notVaccinated Skiplist */
                    removeFromSkipList(notVaccSkipListMap, newCitizen, newCitizen->country);
                    
                    /* Insert on Bloom Filter for this virus */
                    insertCitizenInBloomFilter(bloomSize, bloomFilterMap, newCitizen);

                    /* Insert on vaccinatedSkipList */
                    insertCitizenInVaccSkipList(vaccSkipListMap, newCitizen);
                    
                    /* Insert on citizen map */
                    map_insert(citizenMap, hashKey, newCitizen);
                    printGreen("INSERTED\n");
                    
                }
            }
        }
        else{
            /* If record does not exists on citizenMap */
            /* Is about a NEW record */
            map_insert(citizenMap, hashKey, newCitizen);

            /* Update the population of the new citizen's [COUNTRY] */
            updatePopulation(distinctCitizens, countryPopulationMap, newCitizen);

            /* Update with the new [VIRUS] and [COUNTRY] the corresponding Lists.*/
            updateVirusList(virusesList, newCitizen);
            updateCountyList(countriesList, newCitizen);

            if(strcmp(newCitizen->isVaccinated,"YES")==0 || strcmp(newCitizen->isVaccinated,"YES\n")==0 ){

                /* Vaccinated record insert on the Bloom Filter for the citzen's [VIRUS] */
                insertCitizenInBloomFilter(bloomSize, bloomFilterMap, newCitizen);
            
                /* Vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS] */
                insertCitizenInVaccSkipList(vaccSkipListMap, newCitizen);
            }
            else if(strcmp(newCitizen->isVaccinated,"NO\n")==0 || strcmp(newCitizen->isVaccinated,"NO")==0){

                /* [NOT] vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS] */
                insertCitizenInNotVaccSkipList(notVaccSkipListMap, newCitizen);
            }   

            // printCitizen(newCitizen);
            printGreen("INSERTED\n");
        }
        destroyAssistantValues(parsedFirstName, parsedLastName, parsedCountry, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
    }
    return ;
}

void removeFromSkipList(Map SkipListMap, Record citizen, char* virus){

   MapNode m = map_find_node(SkipListMap, virus);

   /* Remove from not vaccinated skiplist */
   if(m != NULL){
      SkipList virusSL =(SkipList) map_node_value(SkipListMap, m);
      skipListNode n = SL_find_node(virusSL, &citizen->ID, compare_IDs);

      if(n!= NULL)
        SL_remove(virusSL, &citizen->ID, compare_IDs);
   }
   else{
      printf("insertCitizenRecord : ERROR. Wrong data parsing. Cannot find the corresponding NOT VACCINATED skipList to remove the old record\n");
   }
}

void vaccinateNow(int numOfArguments,int bloomSize, char str[], char* arr[], Map citizenMap, Map vaccSkipListMap, Map notVaccSkipListMap, Map distinctCitizens, Map bloomFilterMap, Map countryPopulationMap, LinkedList countriesList, LinkedList virusesList ){
    
    if(numOfArguments == 6){

        int parsedID = -1, parsedAge = -1  ;
        char *parsedFirstName = NULL, *parsedLastName = NULL, *parsedCountry = NULL, *parsedVirusName = NULL, *parsedIsVaccinated = NULL ; 
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);
        
        
        int searchID ;
        searchID = atoi(arr[1]);

        parsedID = atoi(arr[1]);
        parsedFirstName = strdup(arr[2]);
        parsedLastName = strdup(arr[3]);
        parsedCountry = strdup(arr[4]);
        parsedAge =  atoi(arr[5]);
        parsedVirusName = strdup(arr[6]);
        removeChar(parsedVirusName, '\n');

        /* We want to search if the given citizen (ID, virus) exists in the Citizen Map */
        struct hash_rec hashKey ;
        hashKey.ID = &parsedID;
        hashKey.virus = parsedVirusName;
        MapNode m = map_find_node(citizenMap, &hashKey);

        /* And if exists */
        if(m != NULL){
            
            Record citizen = NULL;

            /* Search for citizen in [VACCINATED] Skip List */
            citizen = searchForCitizenInVaccSkipList(&searchID, vaccSkipListMap, parsedVirusName);
            if (citizen != NULL){
                /* If citizen is already vaccinated for the given VIRUS */
                printf("ERROR: CITIZEN %d ALREADY VACCINATED ON ", searchID);
                printDate(citizen->dateVaccinated);
                return ;
            }
            else{

            /* Searh for citizen in [NOT VACCINATED] Skip List and if exists remove it*/
                citizen = searchForCitizenInNotVaccSkipList(&searchID, notVaccSkipListMap, parsedVirusName);
                if(citizen != NULL)
                    removeFromSkipList(notVaccSkipListMap, citizen, parsedVirusName);
            }

            /* -------------------------------- VACCINATE existing citizen -------------------------------------------- */
            
            /* Change citizen's isVaccinated value to "YES" */
            free(citizen->isVaccinated);
            citizen->isVaccinated = strdup("YES");
            
            /* Initialize dateVaccinated part */
            citizen->dateVaccinated = malloc(sizeof(*citizen->dateVaccinated));

            /* Initialize day */
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            citizen->dateVaccinated->day = tm.tm_mday;
            citizen->dateVaccinated->month = tm.tm_mon + 1;
            citizen->dateVaccinated->year = tm.tm_year + 1900; 
            
            /* vaccinated record insert on the Bloom Filter for the citzen's [VIRUS] */
            insertCitizenInBloomFilter(bloomSize, bloomFilterMap, citizen);
            
            /* vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS] */
            insertCitizenInVaccSkipList(vaccSkipListMap, citizen);

            /* Free used value */
            if(parsedVirusName != NULL) free(parsedVirusName);

            printGreen("VACCINATED\n");

        }
        else {

            /* -------------------------------- VACCINATE new citizen ------------------------------------------------ */
            Record newCitizen = malloc(sizeof(*newCitizen));
            newCitizen->ID = parsedID;
            newCitizen->firstName = strdup(parsedFirstName);
            newCitizen->lastName = strdup(parsedLastName);
            newCitizen->country = strdup(parsedCountry);
            newCitizen->age = parsedAge;
            newCitizen->virusName = parsedVirusName;
            newCitizen->isVaccinated = strdup("YES");
            newCitizen->dateVaccinated = malloc(sizeof(*newCitizen->dateVaccinated));

            /* Initialize day */
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            newCitizen->dateVaccinated->day = tm.tm_mday;
            newCitizen->dateVaccinated->month = tm.tm_mon + 1;
            newCitizen->dateVaccinated->year = tm.tm_year + 1900; 
            newCitizen->dateVaccinated->day = tm.tm_mday;
            newCitizen->dateVaccinated->month = tm.tm_mon + 1;
            newCitizen->dateVaccinated->year = tm.tm_year + 1900;     

            hashRec hashKey = initializeHashKey(newCitizen->ID, newCitizen->virusName);


            /* Update with the new [VIRUS] and [COUNTRY] the corresponding Lists ------------------ */
            updateVirusList(virusesList, newCitizen);
            updateCountyList(countriesList, newCitizen);

            /* Vaccinated record insert on the Bloom Filter for the citzen's [VIRUS] */
            insertCitizenInBloomFilter(bloomSize, bloomFilterMap, newCitizen);
            
            /* Vaccinated citizen insert on the Vaccinated Skip List for the citizen's [VIRUS] */
            insertCitizenInVaccSkipList(vaccSkipListMap, newCitizen);

            /* Update population */
            updatePopulation(distinctCitizens, countryPopulationMap, newCitizen);

            /* Insert new record on map  */
            map_insert(citizenMap, hashKey, newCitizen);

            printGreen("VACCINATED\n");
        }

        if(parsedFirstName != NULL) free(parsedFirstName);
        if(parsedLastName != NULL) free(parsedLastName);
        if(parsedCountry != NULL) free(parsedCountry);
        if(parsedIsVaccinated != NULL) free(parsedIsVaccinated);
        
    }
    
    return ;

}

void listNonVaccinatedPersons(int numOfArguments, char str[], char* arr[], Map notVaccSkipListMap){
    if(numOfArguments ==1 ){
        /* Put the arguments user inserted into a new array that will contain them */
        parseValues(str, arr);


        char* virus = strdup(arr[1]);

        /* The virus contains the endline character, too */
        removeChar(virus, '\n');

        /* Get the not vaccinated Skip List for this virus */
        MapNode m = map_find_node(notVaccSkipListMap, virus);
        if(m != NULL){
            SkipList virusSL = map_node_value(notVaccSkipListMap, m);
            
            /* Iterate through the Skip List */
            for(skipListNode node = SL_first(virusSL); node != NULL ; node = SL_next(node)){
                Record citizen = SL_node_val(node);

                /* And print */
                printf("%d %s %s %s %d\n", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age);
            }
        }
        else {
            printf("No Not Vaccinated SkipList for this virus\n");
        }
        if(virus != NULL) free(virus);
        return ;
    }
    return ;
}