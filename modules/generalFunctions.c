


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

/* -------- User-defined header files ------- */
#include "generalFunctions.h"
#include "bloomFilter.h"
#include "common.h"



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
    else if((*(int*)first->ID == *(int*)second->ID))
        return 0;
    // if ( (((hashRec)a)->ID == ((Record)b)->ID ) &&  (strcmp(((Record)a)->virusName, ((Record)b)->virusName) != 0) )
    //     return 0;
    // else if( (((Record)a)->ID == ((Record)b)->ID) &&  (strcmp(((Record)a)->virusName, ((Record)b)->virusName) == 0) && (strcmp(((Record)a)->isVaccinated, ((Record)b)->isVaccinated) != 0))
    //     return 0;
    else
        return 1;

    // return ((Record)a)->ID - ((Record)b)->ID ;

}

int compare_values(Pointer a, Pointer b){
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

void destroy_virus(Pointer value){
    free(value);
}

/* A function that is used to destroy a bloom filter from a map */
void destroy_virus_bf(Pointer rec){
    bf_destroy((BloomFilter)rec);
}

void parseValues(char buffer[], char* array[]){
    
    int i = 0;
    array[i] = strtok(buffer, " ");
    while(array[i] !=NULL ){
        array[++i] = strtok(NULL, " ");
    }

}

ERR_CHK assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, date* dateVaccinated){
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
hashRec initializeHashKey(int ID, char* virus){
    hashRec hashrec = malloc(sizeof(*hashrec));

    hashrec->ID = create_int(ID);
    hashrec->virus = virus;

    return hashrec;


}
Record initializeCitizen(int ID,   char *firstName, char *lastName, char *country, int age, char *virusName, char *isVaccinated, date dateVaccinated){
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
        citizen->dateVaccinated = strdup(dateVaccinated);
    }
 
    return citizen;  

}

void printCitizen(Record citizen){
    if (strcmp(citizen->isVaccinated, "YES") == 0){
        printf("%d %s %s %s %d %s %s %s", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age, citizen->virusName, citizen->isVaccinated, citizen->dateVaccinated);
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


USR_INPT readUserInput(Map bfMap){
    printf("map size %d\n",map_size(bfMap));
    char buf[BUFFER_SIZE];
    char str[BUFFER_SIZE];
    while(fgets(buf, BUFFER_SIZE, stdin) != 0){
        int numOfArguments = countArgs(buf) - 1;
        strcpy(str,buf);
        char *buffer = strtok(buf, " ");
        int *arr[numOfArguments+3];
        if(strcmp("exit\n", buffer) ==0){
            return USR_EXIT;
        }
        else if(strcmp("vaccineStatusBloom", buffer) ==0){
            if (numOfArguments == 2){
                
                parseValues(str, arr);
                if (!checkID(arr[1])) return ARG_ERR; /* invalid id. either alpharethmetic either length >4 */

                char* virus = strdup(arr[2]);
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


                return INPT_1;
            }
            else{
                printf("Given arguments are %d \n",numOfArguments);
                return ARG_ERR;
            }
        }
        else if(strcmp("vaccineStatus", buffer) ==0 || strcmp("vaccineStatus\n", buffer) ==0){
            if (numOfArguments == 2){
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_2;
            }
            else if (numOfArguments == 1) {
                printf("Given arguments are %d \n",numOfArguments);
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
                return INPT_3;
            }
            else if (numOfArguments == 4){
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
                return INPT_4;
            }
            else if (numOfArguments == 4){
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_4;
            }
            else {
                printRed("Usage : ./popStatusByAge [country] virusName date1 date2\n");
                return ARG_ERR;  
            }
            // printf("Given arguments are %d \n",numOfArguments);
        }
        else if(strcmp("insertCitizenRecord", buffer) ==0 || strcmp("insertCitizenRecord\n", buffer) ==0){
            if (numOfArguments == 7){
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_5;
            }
            else if (numOfArguments == 8){
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_5;
            }
            else {
                printRed("Usage : ./insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date] \n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);
           
        }
        else if(strcmp("vaccinateNow", buffer) ==0){
            if (numOfArguments == 6){
                printf("Given arguments are %d \n",numOfArguments);
                return INPT_5;
            }
            else {
                printRed("Usage : ./vaccinateNow citizenID firstName lastName country age virusName \n");
                return ARG_ERR;
            }
            // printf("Given arguments are %d \n",numOfArguments);
            
        }
        else if(strcmp("list-nonVaccinated-Persons", buffer) ==0 || strcmp("list-nonVaccinated-Persons", buffer) ==0){
             if (numOfArguments == 1){
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

