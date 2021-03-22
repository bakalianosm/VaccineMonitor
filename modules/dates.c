/* --------------------------------------------------------
 * ---------------------- DATES ---------------------------
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
#include "dates.h"





Date transformDate(char *dateString){

    Date dateStruct = malloc(sizeof(*dateStruct));

    char *day = strtok(dateString,"-/n");
    char *month = strtok(NULL,"-/n");
    char *year = strtok(NULL,"-/n");

    if (day == NULL || month == NULL || year == NULL){
        printf("Error with date parsing \n");
        free(dateStruct);
        return NULL;
    }

    dateStruct->day = atoi(day);
    dateStruct->month = atoi(month);
    dateStruct->year = atoi(year);

    return dateStruct;
}

void printDate(Date date){

    if (date->day == 0 || date->month == 0 || date->year == 0){
        printRed("Error! NULL field in DATE\n");
        return;
    }
    
    printf("%d-%d-%d\n",date->day, date->month,date->year);
}

int compareDates(Date first, Date second){
    if(first->year > second->year){
        return 1;
    }
    else if (first->year < second->year){
        return -1;
    }
    else {

        if(first->month > second->month){
            return 1;
        }
        else if(first->month < second->month){
            return -1;
        }
        else {

            if(first->day > second->day){
            return 1;
            }
            else if(first->day < second->day){
                return -1;
            }

            else{
                return 0;
            }
        }

    }

    return -1;
}

int checkDateFromString(char* stringDate){

    Date date = transformDate(stringDate);
    if(!(date->day >= 1 && date->day <=30)){
        printRed("Date error : invalid DAY\n");
        free(date);
        return 0;
    }
    if(!(date->month >= 1 && date->month <=12)){
        printRed("Date error : invalid MONTH\n");
        free(date);
        return 0;
    }
    if(!(date->year >= 1 && date->year <=2021)){
        printRed("Date error : invalid YEAR\n");
        free(date);

        return 0;
    }
    free(date);

    return 1;
}

int compareDateStrings(char* from, char* to){
    int ret = 0;
    // Date dateFrom = transformDate(from) ;
    // Date dateTo = transformDate(to);

    // ret = compareDates(dateFrom,dateTo);

    // free(dateFrom);
    // free(dateTo);

    return ret;
}
 /* 

void transformDate(char* date){

    char* array[3];

    int i = 0;
    array[i] = strtok(date, "/");
    while(array[i] !=NULL ){
        array[++i] = strtok(NULL, "/");
    }

    printf("date is %s %s %s\n",array[0], array[1], array[2]);

}
*/
