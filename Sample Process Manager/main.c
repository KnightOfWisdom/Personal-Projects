#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processQueue.h"
#include "usefulConsts.h"
#include "processManager.h"

#define IMPLEMENTS_REAL_PROCESS


int main(int argc, char* argv[]) {
    
    // Set up string pointer storing settings for the program
    char *file_path, *scheduler, *memory_strat, *quantum;

    // Process Command line arguments, assumes arguments passed correctly
    for (int i = 0; i < argc; i++){

        if (strcmp(argv[i], "-f") == 0){
            file_path = argv[i + 1];
        }else if (strcmp(argv[i], "-s") == 0){
            scheduler = argv[i + 1];
        }else if (strcmp(argv[i], "-m") == 0){
            memory_strat = argv[i + 1];
        }else if (strcmp(argv[i], "-q") == 0){
            quantum = argv[i + 1];
        }
    }


    FILE *file = fopen(file_path, "r");

    // Check for scheduler type
    int is_sjf;
    if (strcmp(scheduler, "SJF") == EQUALS){
        is_sjf = TRUE;
    }else if (strcmp(scheduler, "RR") == EQUALS){
        is_sjf = FALSE;
    }else{
        printf("Invalid scheduler");
        exit(EXIT_FAILURE);
    }

    // Check for memory management type 
    int is_finite_mem;
    if (strcmp(memory_strat, "best-fit") == EQUALS){
        is_finite_mem = TRUE;
    }else if (strcmp(memory_strat, "infinite") == EQUALS){
        is_finite_mem = FALSE;
    }else{
        printf("Invalid memory strategy");
        exit(EXIT_FAILURE);
    }

    int int_quantum;
    sscanf(quantum,"%d", &int_quantum);
    process_manager(file, is_sjf, is_finite_mem, int_quantum);
    
    fclose(file);
    return 0;

}



