


/* --------------------------------------------------------
 * ------------------- USER INPUT -------------------------
 * ------------------- HEADER FILE ------------------------
 * --------------------------------------------------------
 */

#pragma once

#include "bloomFilter.h"
#include "linkedList.h"
#include "common.h"
#include "map.h"

/* Read user's input */
void readUserInput(FILE* input_fp, char* recordFile, int bloomSize, Map bloomFilterMap, Map vaccSkipListMap, Map notVaccSkipListMap, LinkedList virusesList, Map countryPopulationMap, LinkedList countriesList, Map citizenMap, Map distinctCitizens);
