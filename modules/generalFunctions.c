


/* --------------------------------------------------------
 * ---------------- GENERAL FUNCTIONS ---------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------- User-defined header files ------- */
#include "generalFunctions.h"
#include "common.h"

int* create_int(int value){
    int *p = malloc(sizeof(int));
    *p = value ;
    return p;

}
int compare_records(Pointer a, Pointer b){
    return (((Record)a)->ID - ((Record)b)->ID);

}

void record_destroy_key(Pointer value){
    free((int*)value);
}

void record_destroy_value(Pointer rec){
    free(((Record)rec)->firstName);
    free(((Record)rec)->lastName);
    free(((Record)rec)->country);
    
    if( strcmp(((Record)rec)->isVaccinated, "YES") == 0)
        free(((Record)rec)->dateVaccinated);

    free(((Record)rec));
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

    if(strcmp(*isVaccinated, "NO\n")==0){
        *isVaccinated = strdup("NO");
    }

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
        printf("%d %s %s %s %d %s %s \n", citizen->ID, citizen->firstName, citizen->lastName, citizen->country, citizen->age, citizen->virusName, citizen->isVaccinated);
    }
}

