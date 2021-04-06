


/* -------------------------------------------------------
 * ----------------- COMMON TYPES ------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once

/* Buffer size for the whole application */
#define BUFFER_SIZE 256

/* User input constants */
#define USR_EXIT 0

#include <stdbool.h>
#include "dates.h"


typedef void* Pointer;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int ERR_CHK;
typedef int (*CompareFunc)(Pointer a, Pointer b);
typedef void (*DestroyFunc)(Pointer value);
typedef void (*PrintFunc)(Pointer value);

typedef unsigned long (*BF_HashFunc)(unsigned char *str, unsigned int i);

/* Record struct that represents the citizens */
struct record {
    int ID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    char* isVaccinated;
    Date dateVaccinated;
};

typedef struct record* Record;

/* A struct that is used as the key for Hashing a Citizen */
struct hash_rec {
    int* ID;
    char* virus;
};

typedef struct hash_rec* hashRec;
