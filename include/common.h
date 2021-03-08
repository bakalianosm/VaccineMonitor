#pragma once

#include <stdbool.h>

typedef void* Pointer;
typedef char* date;
typedef unsigned int uint;
typedef unsigned long ulong;


typedef int (*CompareFunc)(Pointer a, Pointer b);
typedef void (*DestroyFunc)(Pointer value);

typedef unsigned long (*BF_HashFunc)(unsigned char *str, unsigned int i);

struct record {
    int ID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    bool isVaccinated;
    date dateVaccinated;
};

typedef struct record* Record;