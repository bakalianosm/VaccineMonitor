


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

    if ( (*(int*)first->ID == *(int*)second->ID) && (strcmp(first->virus, second->virus) == 0) )
        return 0;
    // else if((*(int*)first->ID == *(int*)second->ID))
    //     return 0;
    // if ( (((hashRec)a)->ID == ((Record)b)->ID ) &&  (strcmp(((Record)a)->virusName, ((Record)b)->virusName) != 0) )
    //     return 0;
    // else if( (((Record)a)->ID == ((Record)b)->ID) &&  (strcmp(((Record)a)->virusName, ((Record)b)->virusName) == 0) && (strcmp(((Record)a)->isVaccinated, ((Record)b)->isVaccinated) != 0))
    //     return 0;
    else
        return 1;

    // return ((Record)a)->ID - ((Record)b)->ID ;

}

int compare_values(Pointer a, Pointer b){
    return ((Record)a)->ID -((Record)b)->ID  ;
}

int compare_IDs(Pointer a, Pointer b){
    return ((Record)a)->ID -((Record)b)->ID ;
}

void record_destroy_key(Pointer value){
    /* free the struct container */
    free(((hashRec)value)->ID);

    /* free the whole struct */
    free((hashRec)value);
}

void record_destroy_value(Pointer rec){
    free(((Record)rec)->firstName);
    free(((Record)rec)->lastName);
    free(((Record)rec)->country);
    free(((Record)rec)->virusName);

    if( strcmp(((Record)rec)->isVaccinated, "YES") == 0){

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

/* A function that is used to destroy a bloom filter from a map */
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
    if( (strcmp(valuesArray[6],"YES") == 0) && ( valuesArray[7] == NULL )){
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

    // if(strcmp(*isVaccinated, "NO\n")==0){
    //     *isVaccinated = strdup("NO");
    // }

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

USR_INPT readUserInput(int bloomSize, Map bfMap, Map vaccSkipListMap, Map notVaccSkipListMap, LinkedList virusesList, Map countryPopulationMap, LinkedList countriesList, Map citizenMap, Map distinctCitizens){

    char buf[BUFFER_SIZE];
    char str[BUFFER_SIZE];
    while(fgets(buf, BUFFER_SIZE, stdin) != 0){
        int numOfArguments = countArgs(buf) - 1;
        strcpy(str,buf);
        char *buffer = strtok(buf, " ");
        int *arr[numOfArguments+2];
        int *searchID = NULL;
        char* firstName = NULL;
        char* lastName = NULL;
        char *virus = NULL;
        char* country = NULL;
        int* age = NULL;
        char* dateFrom = NULL;
        char* dateTo = NULL;
        if(strcmp("exit\n", buffer) ==0){
            if(virus != NULL) free(virus);
            if(searchID != NULL) free(searchID);
            if(firstName != NULL) free(firstName);
            if(lastName != NULL) free(lastName);
            if(age != NULL) free(age);
            if(country != NULL) free(country);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            return USR_EXIT;
        }
        else if(strcmp("vaccineStatusBloom", buffer) ==0){
            if (numOfArguments == 2){

                parseValues(str, arr);
                if (!checkID(arr[1])) return ARG_ERR; /* invalid id. either alpharethmetic either length >4 */

                virus = strdup(arr[2]);
                removeChar(virus,'\n');
                unsigned char *IDstring = (unsigned char*)strdup(arr[1]);
                MapNode m = MAP_EOF;

                m = map_find_node(bfMap, virus);

                if(m != MAP_EOF){
                    BloomFilter virusBF = (BloomFilter)map_node_value(bfMap, m);
                    if(bf_search(virusBF, IDstring)){
                        printf("MAYBE\n");
                    }
                    else{
                        printf("NOT VACCINATED\n");
                    }

                }
                else {
                    printRed("Sorry no BF for this virus. \n");
                }
                if(virus != NULL) free(virus);
                if(IDstring != NULL) free(IDstring);
                return INPT_1;

            }
            else{
                printf("Given arguments are %d \n",numOfArguments);
                return ARG_ERR;
            }
        }
        else if(strcmp("vaccineStatus", buffer) ==0 || strcmp("vaccineStatus\n", buffer) ==0){
            if (numOfArguments == 2){
                // printf("Given arguments are %d \n",numOfArguments);
                parseValues(str, arr);
                if (!checkID(arr[1])) return ARG_ERR; /* invalid id. either alpharethmetic either length >4 */

                virus = strdup(arr[2]);
                removeChar(virus,'\n');

                // printf("args are %s %s",arr[1],arr[2]);
                // printf("%s",virus);
                MapNode m = MAP_EOF;
                searchID = create_int(atoi(arr[1]));
                m = map_find_node(vaccSkipListMap, virus );
                if(m != MAP_EOF){
                    skipListNode sl = NULL;
                    SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap,m);
                    sl = SL_find_node(virusSL,searchID,compare_values);
                    if(sl != NULL){
                        Record vaccinatedCitizen = SL_node_val(sl);
                        if(vaccinatedCitizen->dateVaccinated != NULL){
                            printf("VACCINATED ON ");
                            printDate(vaccinatedCitizen->dateVaccinated);

                        }
                        else{
                            printf("Wrong data parsing. Invalid record \n");
                        }
                    }
                    else{
                        printf("NOT VACCINATED\n");
                    }

                }
                else{
                    printf("NOT VACCINATED\n");
                }
                
                return INPT_2;
            }
            else if (numOfArguments == 1) {
                // printf("Given arguments are %d \n",numOfArguments);
                parseValues(str, arr);
              
                removeChar(arr[1],'\n');
                searchID = create_int(atoi(arr[1]));
                
                if (*searchID > 9999) return ARG_ERR;
                for (ListNode node = LL_first(virusesList) ; node != NULL ; node = LL_next(node)  ){
                    virus = (char *)LL_node_val(node);
                    MapNode m = MAP_EOF;
                    m = map_find_node(vaccSkipListMap, virus );
                    if(m != MAP_EOF){
                        skipListNode sl = NULL;
                        SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap,m);
                        sl = SL_find_node(virusSL,searchID,compare_values);
                        if(sl != NULL){
                            Record vaccinatedCitizen = SL_node_val(sl);
                            if(vaccinatedCitizen->dateVaccinated != NULL){
                                removeChar(vaccinatedCitizen->dateVaccinated, "\n");
                                printf("%s YES ", virus);
                                printDate(vaccinatedCitizen->dateVaccinated);
                            }
                            else{
                                printf("Wrong data parsing. Invalid record \n");
                            }
                        }
                    }
                    m = map_find_node(notVaccSkipListMap, virus );
                    if(m != MAP_EOF){
                        skipListNode sl = NULL;
                        SkipList virusSL = (SkipList)map_node_value(notVaccSkipListMap,m);
                        sl = SL_find_node(virusSL,searchID,compare_values);
                        if(sl != NULL){
                            Record vaccinatedCitizen = SL_node_val(sl);
                            if(vaccinatedCitizen->isVaccinated != NULL){
                                printf("%s NO\n", virus);
                            }
                            else{
                                printf("Wrong data parsing. Invalid record \n");
                            }
                        }
                    }
                    
                }
                if(searchID != NULL) free(searchID);
                return INPT_2;
            }
            else {
                printRed("Usage : ./vaccineStatus citizenID [virusName]\n");
                return ARG_ERR;
            }
        
        }
        else if(strcmp("populationStatus", buffer) ==0 || strcmp("populationStatus\n", buffer) ==0){
            if (numOfArguments == 3){
                printf("Given arguments are %d \n",numOfArguments);

                parseValues(str, arr);
                virus = strdup(arr[1]);
                dateFrom = strdup(arr[2]);
                dateTo = strdup(arr[3]);
                Date from = transformDate(dateFrom);
                Date to = transformDate(dateTo);
                // if (!checkDateFromString(dateFrom)) return ARG_ERR;
                // if (!checkDateFromString(dateTo)) return ARG_ERR;

                // if (compareDateStrings(dateFrom,dateTo) > 0) return ARG_ERR;
                if (compareDates(from,to) > 0) {
                    printRed("populationStatus : Error! Invalid dates\n");
                    return ARG_ERR;
                }

                /* iterate through the list of viruses */
                for(ListNode node = LL_first(countriesList) ; node != NULL ; node = LL_next(node)){
                    country = LL_node_val(node);
                    // printf("countries %s\n",country);
                    MapNode m = MAP_EOF;
                    m = map_find_node(countryPopulationMap, country);
                    if(m != MAP_EOF){
                    int* pop = map_node_value(countryPopulationMap, m);
                    /* country's population */
                    int population = *pop;
                    
                    /* vaccinated people on coyntry */
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
                    // else{
                    //     printRed("Error. No vaccinated records for this country and virus!\n");
                    // }
                  

                    // printf("country population is %d\n", population);
                    float percentage = (float)countryVaccinatedPeople/population * 100;
                    printf("%s %d %.2f%%\n",country, countryVaccinatedPeople,percentage );

                }
                    
                }


                if (virus != NULL) free(virus);
                if (dateFrom != NULL) free(dateFrom);
                if (dateTo != NULL) free(dateTo);
                if (from != NULL) free(from);
                if (to != NULL) free(to);

                return INPT_3;
            }
            else if (numOfArguments == 4){
                
                parseValues(str, arr);
                country = strdup(arr[1]);
                virus = strdup(arr[2]);
                dateFrom = strdup(arr[3]);
                dateTo = strdup(arr[4]);
                Date from = transformDate(dateFrom);
                Date to = transformDate(dateTo);
                // if (!checkDateFromString(dateFrom)) return ARG_ERR;
                // if (!checkDateFromString(dateTo)) return ARG_ERR;

                // if (compareDates(dateFrom,dateTo) > 0) return ARG_ERR;

                if (compareDates(from,to) > 0) {
                    printRed("populationStatus : Error! Invalid dates\n");
                    return ARG_ERR;
                }
                MapNode m = MAP_EOF;
                m = map_find_node(countryPopulationMap, country);
                if(m != MAP_EOF){
                    int* pop = map_node_value(countryPopulationMap, m);
                    /* country's population */
                    int population = *pop;
                    
                    /* vaccinated people on coyntry */
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
                    else{
                        printRed("Error. No vaccinated records for this country and virus!\n");
                    }
                  

                    // printf("country population is %d\n", population);
                    float percentage = (float)countryVaccinatedPeople/population * 100;
                    printf("%s %d %.2f%%\n",country, countryVaccinatedPeople,percentage );

                }
                else{
                    printf("ERROR NOT FOUND THAT COUNTRY\n");
                }
                


                if (virus != NULL) free(virus);
                if (dateFrom != NULL) free(dateFrom);
                if (dateTo != NULL) free(dateTo);
                if (country != NULL) free(country);
                if (from != NULL) free(from);
                if (to != NULL) free(to);
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_3;
            }
            else {
                printRed("Usage : ./populationStatus [country] virusName date1 date2\n");
                return ARG_ERR;
            }
        }
        else if(strcmp("popStatusByAge", buffer) ==0 || strcmp("popStatusByAge\n", buffer) ==0){
            if (numOfArguments == 3){
                
                
                printf("Given arguments are %d \n",numOfArguments);
                parseValues(str, arr);
                virus = strdup(arr[1]);
                dateFrom = strdup(arr[2]);
                dateTo = strdup(arr[3]);
                Date from = transformDate(dateFrom);
                Date to = transformDate(dateTo);
                if (compareDates(from,to) > 0) {
                    printRed("populationStatus : Error! Invalid dates\n");
                    return ARG_ERR;
                }

                /* iterate through the list of viruses */
                for(ListNode node = LL_first(countriesList) ; node != NULL ; node = LL_next(node)){
                    int ageArr[4] = {0, 0, 0, 0};
                    country = LL_node_val(node);
                    printf("%s\n",country);
                    MapNode m = MAP_EOF;
                    m = map_find_node(countryPopulationMap, country);
                    if(m != MAP_EOF){
                        int* pop = map_node_value(countryPopulationMap, m);
                        /* country's population */
                        int population = *pop;
                        
                        /* vaccinated people on coyntry */
                        int countryVaccinatedPeople = 0;
                        
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
                    else {
                        printf("popStatusByAge: Error! Invalid country name or does not exist\n");
                    }
                    printf("\n");
                }
                



                if (virus != NULL) free(virus);
                if (dateFrom != NULL) free(dateFrom);
                if (dateTo != NULL) free(dateTo);
                if (from != NULL) free(from);
                if (to != NULL) free(to);

                return INPT_4;
            }
            else if (numOfArguments == 4){
                printf("Given arguments are %d \n",numOfArguments);

                parseValues(str, arr);
                country = strdup(arr[1]);
                virus = strdup(arr[2]);
                dateFrom = strdup(arr[3]);
                dateTo = strdup(arr[4]);
                Date from = transformDate(dateFrom);
                Date to = transformDate(dateTo);

                if (compareDates(from,to) > 0) {
                    printRed("populationStatus : Error! Invalid dates\n");
                    return ARG_ERR;
                }
                int ageArr[4] = {0, 0, 0, 0};
                MapNode m = MAP_EOF;
                m = map_find_node(countryPopulationMap, country);
                if(m != MAP_EOF){
                    int* pop = map_node_value(countryPopulationMap, m);
                    /* country's population */
                    int population = *pop;
                    
                    /* vaccinated people on coyntry */
                    int countryVaccinatedPeople = 0;
                    
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
                else {
                    printf("popStatusByAge: Error! Invalid country name or does not exist\n");
                }
                if (country != NULL) free(country);
                if (virus != NULL) free(virus);
                if (dateFrom != NULL) free(dateFrom);
                if (dateTo != NULL) free(dateTo);
                if (from != NULL) free(from);
                if (to != NULL) free(to);

                return INPT_4;
            }
            else {
                printRed("Usage : ./popStatusByAge [country] virusName date1 date2\n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);
        }
        else if(strcmp("insertCitizenRecord", buffer) ==0 || strcmp("insertCitizenRecord\n", buffer) ==0){
            if (numOfArguments == 7 || numOfArguments ==8){
                char *array[10] = {NULL, NULL, NULL , NULL , NULL, NULL , NULL, NULL, NULL, NULL} ;
                
                printf("Given arguments are %d \n",numOfArguments);

                int parsedID = -1 ;
                char *parsedFirstName = NULL;
                char *parsedLastName = NULL ;
                char *parsedCountry = NULL ;
                int parsedAge = -1 ;
                char *parsedVirusName = NULL ;
                char *parsedIsVaccinated = NULL ;
                char *parsedDateVaccinated = NULL ;

                parseValues(str,array);

                ERR_CHK error = assignValuesForIoption(array,&parsedID, &parsedFirstName, &parsedLastName, &parsedCountry, &parsedAge, &parsedVirusName, &parsedIsVaccinated, &parsedDateVaccinated);

                
                /* if the above function return an error code , we have to ignore this record */
                if(error != NO_ERROR){
                    if(parsedFirstName !=NULL) free(parsedFirstName);
                    if(parsedLastName !=NULL) free(parsedLastName);
                    if(parsedCountry !=NULL) free(parsedCountry);
                    if(parsedVirusName !=NULL) free(parsedVirusName);
                    if(parsedIsVaccinated !=NULL) free(parsedIsVaccinated);
                    if(parsedDateVaccinated!=NULL) free(parsedDateVaccinated);
                    printf("insertCitizenRecord : ERROR data for insertion\n");
                    return ARG_ERR;
                } 

                /* create a new record */
                Record newCitizen = initializeCitizen(parsedID, parsedFirstName, parsedLastName, parsedCountry, parsedAge, parsedVirusName, parsedIsVaccinated, parsedDateVaccinated);
                hashRec hashKey = initializeHashKey(newCitizen->ID, newCitizen->virusName);
                printCitizen(newCitizen);

                MapNode toSearch = map_find_node(citizenMap, hashKey);
                


            

                /* if record exists on citizenMap */
                if (toSearch != NULL){
                    printf("Exists a record with this ID and VIRUS\n");

                    Record citizenOnMap = (Record)map_node_value(citizenMap, toSearch);
                    
                    /* if the record is vaccinated print error */
                    if (strcmp(citizenOnMap->isVaccinated, "YES") == 0){
                        printf("ERROR: CITIZEN %d ALREADY VACCINATED ON ", citizenOnMap->ID);
                        printDate(citizenOnMap->dateVaccinated);
                        record_destroy_key(hashKey);
                        record_destroy_value(newCitizen);
                    }
                    else {

                        if(strcmp(newCitizen->isVaccinated, "YES") == 0 ){
                      
                            
                            printf("%s",newCitizen->virusName);
                            MapNode m = map_find_node(notVaccSkipListMap, newCitizen->virusName);
                            printCitizen(newCitizen);
                            /* remove from not vaccinated skiplist */
                            if(m != NULL){
                                SkipList virusSL =(SkipList) map_node_value(notVaccSkipListMap, m);
                                SL_remove(virusSL, &citizenOnMap->ID, compare_IDs);
                                printf("Successfully removed from notVaccinatedSkiplist \n");
                            }
                            else{
                                printf("insertCitizenRecord : ERROR. Wrong data parsing. Cannot find the corresponding NOT VACCINATED skipList to remove the old record\n");
                            }

                            /* insert on vaccinatedSkipList */
                            m = map_find_node(vaccSkipListMap, newCitizen->virusName);

                            /* check if exists skiplist for vaccinated records in VACCINATED skiplists */
                            if(m != NULL){

                                /* exists vacc skiplist for this virus */
                                SkipList virusSL = map_node_value(vaccSkipListMap, m);
                               
                                SL_insert(virusSL, &newCitizen->ID, newCitizen, compare_IDs);
                            
                                printf("Successfully inserted in this SKIPLIST\n");

                            }
                            else{
                                printf("Going to create a new SkipList for VACCINATED records\n");
                                /* does not exists vacc skip list for this virus */


                                /* create the skiplist */
                                SkipList virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);

                                /* insert record on skiplist */
                                
                                SL_insert(virusSkipList, &newCitizen->ID ,newCitizen, compare_IDs);
                                skipListNode n = SL_find_node(virusSkipList, &newCitizen->ID, compare_IDs);
                                if (n != NULL) printGreen("Successfully inserted on VACCINATED SKIPLIST \n");
                                printf("for %s \n",newCitizen->virusName);
                                /* insert skiplist on map with skiplists */
                                map_insert(vaccSkipListMap, newCitizen->virusName,virusSkipList);
                            }
                            

                            m = map_find_node(bfMap, newCitizen->virusName);

                            char str[10];
                            sprintf(str, "%d", newCitizen->ID);
                            unsigned char* idString = (unsigned char*)strdup(str);
                            printf("idstringf is %s", idString);
                            if (m != NULL){
                                /* if exists Bloom Filter for this virus */
                                BloomFilter virusBloomFilter = (BloomFilter)map_node_value(bfMap, m);

                                bf_insert(virusBloomFilter, idString);
                            }
                            else{
                                /* if not exists Bloom Filter for this virus */

                                /* Create a Bloom Filter */
                                BloomFilter virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

                                /* insert ID on Bloom Filter */
                                bf_insert(virusBloomFilter, idString);
        
                                /* insert the bloomFilter in the map */
                                map_insert(bfMap, newCitizen->virusName, virusBloomFilter);

                            }

                            free(idString);
                            map_insert(citizenMap, hashKey, newCitizen);
                            printGreen("INSERTED\n");
                            printCitizen(newCitizen);
                            
                        }
                    }


                }
                /* if record does not exists on citizenMap */
                
                else{
                    printf("This is a new record\n");
                
                    map_insert(citizenMap, hashKey, newCitizen);
                    

                    /* ----------------- COUNTRY POPULATION INSERT --------------------- */

                    /* increment the country population only if the record does not exists for
                    any other virus list */
                    MapNode toFind = map_find_node(distinctCitizens, &newCitizen->ID);
                    if (toFind == NULL){
                        map_insert(distinctCitizens,&newCitizen->ID, newCitizen);

                        MapNode m = MAP_EOF;
                        m = map_find_node(countryPopulationMap, newCitizen->country);
                        if(m != MAP_EOF){
                            printf("This country %s exists\n", newCitizen->country);
                            int* population = map_node_value(countryPopulationMap, m);
                            (*population)++;
                        }
                        else{
                            map_insert(countryPopulationMap, newCitizen->country, create_int(1));
                        }
                    }


                    ListNode lnode = NULL;

                    /* insert virus on the list with the viruses if not extists */
                    lnode = LL_find_node(virusesList, newCitizen->virusName, compare_viruses);
                    if (lnode == NULL){
                        printf("This virus does not exist %s\n",newCitizen->virusName);
                        LL_insert_after(virusesList, NULL, newCitizen->virusName);
                    }

                    /* insert virus on the list with the countries if not extists */
                    lnode = LL_find_node(countriesList, newCitizen->country, compare_countries);
                    if (lnode == NULL){
                        printf("This country does not exist %s\n",newCitizen->country);
                        LL_insert_after(countriesList, NULL, newCitizen->country);
                    }

                
                    if(strcmp(newCitizen->isVaccinated,"YES")==0 || strcmp(newCitizen->isVaccinated,"YES\n")==0 ){

                        /* ----------------- VACCINATED RECORD INSERT ON THE BLOOM FILTER FOR THIS VIRUS --------------------- */

                        printf("%d IS VACCINATED FOR %s\n",newCitizen->ID,newCitizen->virusName);


                        MapNode m = MAP_EOF;
                        BloomFilter virusBloomFilter;   

                        /* itransform the id again to string  */
                        char str[10];
                        sprintf(str, "%d", newCitizen->ID);
                        unsigned char* idString = (unsigned char*)strdup(str);
                        m = map_find_node(bfMap,newCitizen->virusName);
                        if (m != MAP_EOF){
                            // printf("Exists this virus\n");
                            virusBloomFilter = (BloomFilter)map_node_value(bfMap, m);
                            if (virusBloomFilter == NULL) { printf("Error!\n") ; exit(EXIT_FAILURE); } 

                            bf_insert(virusBloomFilter, idString);
                            // printf("%s inserted in BF for %s\n",idString, newCitizen->virusName);

                        }
                            else {

                                // printf("Dont exists this virus %s. Going to insert it\n",newCitizen->virusName);

                                /* create a bloom filter for this virus */
                                virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

                                bf_insert(virusBloomFilter, idString);
                                // printf("Created bf for %s\n",newCitizen->virusName);
                                // printf("%s inserted in BF for %s\n",idString, newCitizen->virusName);
                                /* insert the bloomFilter in the map */

                                map_insert(bfMap, newCitizen->virusName, virusBloomFilter);

                            }

                        free(idString);

                        /* ----------------- VACCINATED RECORD INSERT ON [VACCINATED] SKIP LIST FOR THIS VIRUS --------------------- */
                        SkipList virusSkipList;
                        m = map_find_node(vaccSkipListMap, newCitizen->virusName);


                        /* check if exist skip list with this virus */
                        if (m != MAP_EOF){
                        /* if exists insert newCitizen in this skiplist*/
                            virusSkipList = (SkipList)map_node_value(vaccSkipListMap,m);
                            if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

                            /* compare values is compare ID */
                            SL_insert(virusSkipList, &newCitizen->ID,newCitizen, compare_IDs);
                            // printf("%d inserted in %s VACCINATED skiplist\n", newCitizen->ID, newCitizen->virusName);
                        }
                        else{
                        /* if dont exists create skiplist, insert record */

                            /* create a skiplist for this virus */
                            virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);
                            /* compare values is compare ID */

                            SL_insert(virusSkipList, &newCitizen->ID, newCitizen, compare_IDs);
                            // printf("%d inserted in NEW %s VACCINATED skiplist\n", newCitizen->ID, newCitizen->virusName);

                            map_insert(vaccSkipListMap, newCitizen->virusName ,virusSkipList);

                        }

                    }
                    else if(strcmp(newCitizen->isVaccinated,"NO\n")==0 || strcmp(newCitizen->isVaccinated,"NO")==0){
                        /* if the newCitizen is NOT VACCINATED */

                        /* -------------- NOT VACCINATED RECORD INSERT ON THE BLOOM FILTER FOR THIS VIRUS --------------------- */

                        SkipList virusSkipList;
                        MapNode m = map_find_node(notVaccSkipListMap, newCitizen->virusName);


                        /* check if exist skip list with this virus */
                        if (m != MAP_EOF){
                        /* if exists insert newCitizen in this skiplist*/
                            virusSkipList = (SkipList)map_node_value(notVaccSkipListMap,m);
                            if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

                            /* compare values is compare ID */
                            SL_insert(virusSkipList, &newCitizen->ID ,newCitizen, compare_IDs);
                            // printf("%d inserted in %s VACCINATED skiplist\n", newCitizen->ID, newCitizen->virusName);
                        }
                        else{
                        /* if dont exists create skiplist, insert record */

                            /* create a skiplist for this virus */
                            virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);
                            /* compare values is compare ID */

                            SL_insert(virusSkipList, &newCitizen->ID ,newCitizen, compare_IDs);
                            // printf("%d inserted in NEW %s **NOT** VACCINATED skiplist\n", newCitizen->ID, newCitizen->virusName);

                            map_insert(notVaccSkipListMap, newCitizen->virusName,virusSkipList);

                        }
                    }
                }
                
                if(parsedFirstName !=NULL) free(parsedFirstName);
                if(parsedLastName !=NULL) free(parsedLastName);
                if(parsedCountry !=NULL) free(parsedCountry);
                if(parsedVirusName !=NULL) free(parsedVirusName);
                if(parsedIsVaccinated !=NULL) free(parsedIsVaccinated);
                if(parsedDateVaccinated!=NULL) free(parsedDateVaccinated);
                return INPT_5;
            }
            else {
                printRed("Usage : ./insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date] \n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);

        }
        else if(strcmp("vaccinateNow", buffer) ==0){
            if (numOfArguments == 7){
                
                

              
                parseValues(str,arr);
                virus = strdup(arr[6]);
                searchID = create_int(atoi(arr[1]));

                MapNode m ;
                
                /* let's check if is already vaccinated */
                printf("virus %s\n", virus);
                m = map_find_node(vaccSkipListMap, virus);

                if(m != NULL){

                    /* exists a skiplist for this virus */
           

                    SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap, m);
                    /* search if is already vaccinated */
                    skipListNode n = SL_find_node(virusSL, searchID, compare_IDs);

                   
                    if (n= NULL){
                        Record vaccinatedCitizen = SL_node_val(n);
                        printf("ERROR: CITIZEN %d ALREADY VACCINATED ON ", *searchID);
                        printDate(vaccinatedCitizen->dateVaccinated);
                    }
                }
                
                
               
                    
                /* check if exists a not vaccinated list for this virus
                    and if exist this id into it */

                m = map_find_node(notVaccSkipListMap, virus);
                if ( m != NULL){
                    SkipList virusSL = (SkipList)map_node_value(vaccSkipListMap, m);
                    skipListNode n = SL_find_node(virusSL, searchID, compare_IDs);
                    if (n != NULL){
                        SL_remove(virusSL, searchID, compare_IDs);
                    }
                }
                
                /* create a new record */

                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
    
                Record newCitizen = malloc(sizeof(newCitizen));
                newCitizen->ID = atoi(arr[1]);
                newCitizen->firstName = strdup(arr[2]);
                newCitizen->lastName = strdup(arr[3]);
                newCitizen->country =strdup(arr[4]);
                newCitizen->age = atoi(arr[5]);
                newCitizen->virusName = strdup(arr[6]);
                newCitizen->isVaccinated = strdup("YES");
                newCitizen->dateVaccinated = malloc(sizeof(*newCitizen->dateVaccinated));

                newCitizen->dateVaccinated->day = tm.tm_mday;
                newCitizen->dateVaccinated->month = tm.tm_mon + 1;
                newCitizen->dateVaccinated->year = tm.tm_year + 1900;

                ListNode lnode;

                /* insert virus on the list with the viruses if not extists */
                lnode = LL_find_node(virusesList, newCitizen->virusName, compare_viruses);
                if (lnode == NULL){
                    LL_insert_after(virusesList, NULL, newCitizen->virusName);
                }


                /* itransform the id again to string  */
                char str[10];
                sprintf(str, "%d", newCitizen->ID);
                unsigned char* idString = (unsigned char*)strdup(str);
                m = map_find_node(bfMap,newCitizen->virusName);
                if (m != MAP_EOF){
                    // printf("Exists this virus\n");
                    BloomFilter virusBloomFilter = (BloomFilter)map_node_value(bfMap, m);
                    if (virusBloomFilter == NULL) { printf("Error!\n") ; exit(EXIT_FAILURE); } 

                    bf_insert(virusBloomFilter, idString);
                    // printf("%s inserted in BF for %s\n",idString, newCitizen->virusName);

                }
                else {

                    /* create a bloom filter for this virus */
                    BloomFilter virusBloomFilter = bf_create(K_FOR_BF, bloomSize, hash_i);

                    bf_insert(virusBloomFilter, idString);
                  
                    /* insert the bloomFilter in the map */
                    map_insert(bfMap, newCitizen->virusName, virusBloomFilter);

                }

                free(idString);
                /* ----------------- VACCINATED RECORD INSERT ON [VACCINATED] SKIP LIST FOR THIS VIRUS --------------------- */
                SkipList virusSkipList;
                m = map_find_node(vaccSkipListMap, newCitizen->virusName);


                /* check if exist skip list with this virus */
                if (m != MAP_EOF){
                /* if exists insert newCitizen in this skiplist*/
                    virusSkipList = (SkipList)map_node_value(vaccSkipListMap,m);
                    if(virusSkipList == NULL) { printf("Error!\n"); exit(EXIT_FAILURE); }

                    /* compare values is compare ID */
                    SL_insert(virusSkipList, &newCitizen->ID,newCitizen, compare_IDs);

                }
                else{
                /* if dont exists create skiplist, insert record */

                    virusSkipList = SL_create(MAX_SKIPLIST_LEVEL,NULL,NULL);

                    SL_insert(virusSkipList, &newCitizen->ID, newCitizen, compare_IDs);
                 
                    map_insert(vaccSkipListMap, newCitizen->virusName ,virusSkipList);

                }


                
                
                

            }
            else {
                printRed("Usage : ./vaccinateNow citizenID firstName lastName country age virusName \n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);

        }
        else if(strcmp("list-nonVaccinated-Persons", buffer) ==0 || strcmp("list-nonVaccinated-Persons", buffer) ==0){
            if (numOfArguments == 2){
                parseValues(str, arr);
                virus = strdup(arr[1]);
                
                MapNode m = map_find_node(notVaccSkipListMap, virus);
                if(m != NULL){
                    SkipList virusSL = map_node_value(notVaccSkipListMap, m);
                    
                    for(skipListNode node = SL_first(virusSL); node != NULL ; node = SL_next(node)){
                        Record citizen = SL_node_val(node);
                        printf("%d %s %s %s %d\n", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age);
                    }
                }
                else {
                    printf("No NonVaccinated SkipList for this virus\n");
                }
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_7;
            }
            else {
                printRed("Usage : ./list-nonVaccinated-Persons virusName \n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);
        }
        else return INVALID_INPT;
    }

    return 1;
}
