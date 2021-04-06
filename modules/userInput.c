


/* --------------------------------------------------------
 * ------------------- USER INPUT -------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* -------- User-defined header files ------- */
#include "common.h"
#include "unserInput.h"
#include "queryFunctions.h"
#include "generalFunctions.h"

void readUserInput(FILE* input_fp, char* recordFile, int bloomSize, Map bloomFilterMap, Map vaccSkipListMap, Map notVaccSkipListMap, LinkedList virusesList, Map countryPopulationMap, LinkedList countriesList, Map citizenMap, Map distinctCitizens){
    
    char buf[BUFFER_SIZE];
    char str[BUFFER_SIZE];

    printGreen("Type a new query:\n");
    while(fgets(buf, BUFFER_SIZE, stdin) != 0){
        /* Number of arguments for the QUERY */
        int numOfArguments = countArgs(buf) - 1;
        strcpy(str,buf);
        char *buffer = strtok(buf, " ");
        char* arr[numOfArguments+2];

        /* Some values that will hold the arguments */
        int *searchID = NULL;
        char* firstName = NULL;
        char* lastName = NULL;
        char *virus = NULL;
        char* country = NULL;
        int* age = NULL;
        char* dateFrom = NULL;
        char* dateTo = NULL;

        /* QUERIES */
        if(strcmp("exit\n", buffer) ==0){

            /* Free all values if not empty */
            if(virus != NULL) free(virus);
            if(searchID != NULL) free(searchID);
            if(firstName != NULL) free(firstName);
            if(lastName != NULL) free(lastName);
            if(age != NULL) free(age);
            if(country != NULL) free(country);
            if(dateFrom != NULL) free(dateFrom);
            if(dateTo != NULL) free(dateTo);
            freeAllocatedMemory(input_fp, recordFile, virusesList, countriesList, citizenMap, distinctCitizens, bloomFilterMap, vaccSkipListMap, notVaccSkipListMap, countryPopulationMap);
            exit(EXIT_SUCCESS);
        }
        else if(strcmp("vaccineStatusBloom", buffer) ==0){
            if (numOfArguments == 2){
                vaccineStatusBloom(str, arr, bloomFilterMap);
            }
            else{
                printRed("Usage : ./vaccineStatusBloom citizenID virusName\n");
            }
        }
        else if(strcmp("vaccineStatus", buffer) ==0 || strcmp("vaccineStatus\n", buffer) ==0){
            if (numOfArguments == 1 || numOfArguments ==2 ){
                vaccineStatus(numOfArguments, str, arr, vaccSkipListMap, notVaccSkipListMap, virusesList);
            }
            else{
                printRed("Usage : ./vaccineStatus citizenID [virusName]\n");
            }
        }
        else if(strcmp("populationStatus", buffer) ==0 || strcmp("populationStatus\n", buffer) ==0){
            if (numOfArguments == 3 || numOfArguments == 4){
                populationStatus(numOfArguments, str, arr, vaccSkipListMap, notVaccSkipListMap, countryPopulationMap, countriesList);
            }
            else {
                printRed("Usage : ./populationStatus [country] virusName date1 date2\n");

            }
        }
        else if(strcmp("popStatusByAge", buffer) ==0 || strcmp("popStatusByAge\n", buffer) ==0){
            if (numOfArguments == 3 || numOfArguments ==4){
                popStatusByAge(numOfArguments, str, arr, vaccSkipListMap, notVaccSkipListMap, countryPopulationMap, countriesList);
            }
            else {
                printRed("Usage : ./popStatusByAge [country] virusName date1 date2\n");

            }
        }
        else if(strcmp("insertCitizenRecord", buffer) ==0 || strcmp("insertCitizenRecord\n", buffer) ==0){
            if (numOfArguments == 7 || numOfArguments ==8){
                insertCitizenRecord(numOfArguments, bloomSize, str, arr, citizenMap, vaccSkipListMap, notVaccSkipListMap, distinctCitizens, bloomFilterMap, countryPopulationMap, countriesList, virusesList);
            }
            else {
                printRed("Usage : ./insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date] \n");

            }
        }
        else if(strcmp("vaccinateNow", buffer) ==0){
            if(numOfArguments == 6){
                vaccinateNow(numOfArguments, bloomSize, str, arr, citizenMap, vaccSkipListMap, notVaccSkipListMap, distinctCitizens, bloomFilterMap, countryPopulationMap, countriesList, virusesList);
            }
            else {
                printRed("Usage : ./vaccinateNow citizenID firstName lastName country age virusName \n");
            }

        }
        else if(strcmp("list-nonVaccinated-Persons", buffer) ==0 || strcmp("list-nonVaccinated-Persons", buffer) ==0){
            if (numOfArguments == 1){
                listNonVaccinatedPersons(numOfArguments, str, arr, notVaccSkipListMap);
            }
            else {
                printRed("Usage : ./list-nonVaccinated-Persons virusName \n");

            }
        }
        else{
            printRed("User Input : ERROR. Please type a valid query\n");
        }
        printGreen("\nType a new query:\n");
    }
}