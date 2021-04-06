


/* -------------------------------------------------------
 * --------------- ----- DATES  --------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

struct date {
    int day;
    int month;
    int year;
} ;

typedef struct date* Date;

/*Ccreates a date from a string */
Date transformDate(char *date);

/* Prints a date */
void printDate(Date date);

/* Compares dates */
int compareDates(Date first, Date second);

/* Checks if dateGiven in string is valid */
int checkDateFromString(char* stringDate);

/* Compares to dates from string */
int compareDateStrings(char* from, char* to);