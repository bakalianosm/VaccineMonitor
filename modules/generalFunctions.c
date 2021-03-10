


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

int compare_records(Pointer a, Pointer b){
    return 0;
}

void record_destroy_key(Pointer rec){
    free(((Record)rec)->virusName);
}

void record_destroy_value(Pointer rec){
    free(((Record)rec)->firstName);
    free(((Record)rec)->lastName);
    free(((Record)rec)->country);
    
    if( ((Record)rec)->isVaccinated == true)
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

void assignValues(char* valuesArray[],int *ID,   char **firstName, char **lastName, char **country, int *age, char **virusName, char **isVaccinated, date* dateVaccinated){
    *ID = atoi(valuesArray[0]);
    
    *firstName = strdup(valuesArray[1]);
    *lastName = strdup(valuesArray[2]);
    *country = strdup(valuesArray[3]);
    *age = atoi(valuesArray[4]);
    *virusName = strdup(valuesArray[5]);
    *isVaccinated = strdup(valuesArray[6]);
   
    if (valuesArray[7] != NULL){
        *dateVaccinated = strdup(valuesArray[7]);
    }
    else {
        dateVaccinated = NULL;
    }

    /* -------------------------------- VALIDATION CHECK --------------------------------------------*/
    
    /* age check */
    if(! (*age > 0 && *age <= 120)) {
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);
        if( dateVaccinated != NULL) printf("%s\n", *dateVaccinated);
    }

    /* if vaccinated is no and provided a date */
    if( (strcmp(valuesArray[6],"NO") == 0) && valuesArray[7] != NULL)  
        printf("ERROR IN RECORD %d %s %s %s %d %s %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName,*isVaccinated, *dateVaccinated);

    /* if is vaccinated and not given a date  */
    if( (strcmp(valuesArray[6],"YES") == 0) && ( valuesArray[7] == NULL ))
        printf("ERROR IN RECORD %d %s %s %s %d %s %s \n", *ID, *firstName, *lastName, *country, *age, *virusName, *isVaccinated);

}