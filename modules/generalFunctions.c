


/* --------------------------------------------------------
 * ---------------- GENERAL FUNCTIONS ---------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

/* ----------- System header files  --------- */
#include <stdlib.h>

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




