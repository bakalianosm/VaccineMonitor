#pragma once

/* Buffer size for the whole application */
#define BUFFER_SIZE 256

/* User input constants */
#define USR_EXIT 0
#define INPT_1 1
#define INPT_2 2
#define INPT_3 3
#define INPT_4 4
#define INPT_5 5
#define INPT_6 6
#define INPT_7 7
#define INPT_8 8
#define INVALID_INPT 10
#define ARG_ERR 11

#include <stdbool.h>
#include "dates.h"


typedef void* Pointer;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int ERR_CHK;
typedef int USR_INPT;
typedef int (*CompareFunc)(Pointer a, Pointer b);
typedef void (*DestroyFunc)(Pointer value);
typedef void (*PrintFunc)(Pointer value);

typedef unsigned long (*BF_HashFunc)(unsigned char *str, unsigned int i);


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

struct hash_rec {
    int* ID;
    char* virus;
};

typedef struct hash_rec* hashRec;
