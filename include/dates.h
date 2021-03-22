


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

/* creates a date from a string */
Date transformDate(char *date);

/* prints a date */
void printDate(Date date);

/* compares dates */
int compareDates(Date first, Date second);

/* checks if dateGiven in string is valid */
int checkDateFromString(char* stringDate);

/* compares to dates from string */
int compareDateStrings(char* from, char* to);