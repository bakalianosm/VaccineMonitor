#pragma once

#include <stdbool.h>

typedef void* Pointer;
typedef char* date;
typedef unsigned int uint;

typedef int (*CompareFunction)(Pointer a, Pointer b);
typedef void (*DestroyFunction)(Pointer);

typedef int (*CompareFunc)(Pointer a, Pointer b);
typedef void (*DestroyFunc)(Pointer value);


struct record {
    int ID;
    char *fistName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    bool isVaccinated;
    date dateVaccinated;
};

typedef struct record* Record;