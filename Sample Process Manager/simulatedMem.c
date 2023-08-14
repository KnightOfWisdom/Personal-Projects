/** memManager.c
 * 
 * Memory Manager for Process manager(simulated)
 * Created by Ke Liao (keliao@student.unimelb.edu.au) 10/04/2022
 * 
 * Contain the stricture fpr linked list representing 
 * and allow the management of memory space in the event that memory 
 * is limited. Also contain function for the creation & freeing of 
 * the memory linked list as well as functions facilitating management of
 * memory.
 * 
 * Note: The construction of linked list is inquired by linkedList.c
 * I've done for Assignment 2 of Algorithms and data structures. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "simulatedMem.h"
#include "usefulConsts.h"


struct mem_block{
    int start_address, length, is_free;
    mem_block_t *prev;
    mem_block_t *next;
};

struct sim_mem{
    int total_mem;
    mem_block_t *head;
    mem_block_t *foot;
};


/* Creating new struct representing simulated memory*/
sim_mem_t *new_sim_mem(int mem_size){
    sim_mem_t *sim_mem;
    sim_mem = malloc(sizeof(*sim_mem));
    assert(sim_mem != NULL);
    sim_mem->total_mem = mem_size;
    mem_block_t *whole_memory = new_mem_block(0, mem_size, TRUE);
    sim_mem->head = sim_mem->foot = whole_memory;
    return sim_mem;
}


/* Create a new memory block */
mem_block_t *new_mem_block(int start, int length, int is_free){
    mem_block_t *mem_block;
    mem_block = malloc(sizeof(*mem_block));
    assert(mem_block != NULL);
    mem_block->start_address = start;
    mem_block->length = length;
    mem_block->is_free = is_free;
    mem_block->prev = mem_block->next = NULL;
    return mem_block;
}


/* Change a memory to hole(and merge with other surrounding holes). */
void memory_to_hole(mem_block_t *mem_block, sim_mem_t *sim_mem){
    mem_block->is_free = TRUE;  // Change block to hole
    free_memory_merge(mem_block, sim_mem);
}


/* Merge a hole with surrounding holes*/
void free_memory_merge(mem_block_t *mem_block, sim_mem_t *sim_mem){

    // Only merge holes with holes
    if (mem_block->is_free == FALSE){
        return;
    }

    // Check the next memory block can be merged
    mem_block_t *next_block = mem_block->next;
    if (next_block != NULL){
        if (next_block->is_free == TRUE){

            mem_block->next = next_block->next;
            mem_block->length += next_block->length;

            // deal with case next block beinf the foot of the simulated memory
            if(next_block == sim_mem->foot){
                sim_mem->foot = mem_block;
            }

            free(next_block);
        }
    }

    // now check if any merging needed with previous memory block
    mem_block_t *prev_block = mem_block->prev;
    if (prev_block != NULL){
        if (prev_block->is_free == TRUE){

            mem_block->prev = prev_block->prev;
            mem_block->length += prev_block->length;
            mem_block->start_address = prev_block->start_address;

            // deal with case prev block is head of simulated memory
            if(prev_block == sim_mem->head){
                sim_mem->head = mem_block;
            }

            free(prev_block);
        }
    }
}


/* Create and return the memory block allocated according to best fit. 
Returns NULL in case of failure */
mem_block_t *best_fit_mem_allocate(int mem_req, sim_mem_t *simulated_mem){
    mem_block_t *allocated_mem;

    // Go through the memory block to find best fit
    int curr_best_size = (simulated_mem->total_mem) + 1; 
    mem_block_t *curr_best_block = NULL;
    mem_block_t *curr_block = simulated_mem->head;
    while (curr_block != NULL){

        // Change the selected memory block if its better fit
        if (curr_block->is_free){
            int curr_mem_size = curr_block->length;
            if(curr_mem_size >= mem_req && curr_mem_size < curr_best_size){
                curr_best_block = curr_block;
                curr_best_size = curr_mem_size;
            }
        }

        curr_block = curr_block->next;
    }

    // Now allocate memory(and keep in mind of resulting hole)
    if (curr_best_block != NULL){
        int new_hole_len = curr_best_block->length - mem_req;

        // Create the new hole(and merge it with other free block if needed)
        if (new_hole_len > 0){
            int new_hole_start = curr_best_block->start_address + mem_req;
            mem_block_t *new_hole = new_mem_block(new_hole_start, new_hole_len, 
                                                  TRUE);
            new_hole->next = curr_best_block->next;
            curr_best_block->next = new_hole;
            free_memory_merge(new_hole, simulated_mem);
        }

        curr_best_block->length = mem_req;
        curr_best_block->is_free = FALSE;

    }

    allocated_mem = curr_best_block;
    return allocated_mem;
}


/* Free the simulated memory */
void free_simulated_mem(sim_mem_t *simulated_mem){
    assert(simulated_mem != NULL);
    mem_blocks_free(simulated_mem->head);
    free(simulated_mem);
}


/* Free the memory block and all memory blocks after*/
void mem_blocks_free(mem_block_t *mem_block){
    if (mem_block == NULL){
        return; // No need to free NULL
    }else if (mem_block->next != NULL){
        mem_blocks_free(mem_block->next);
    }

    free(mem_block);
}


/* Get starting address of the memory block */
int get_start_address(mem_block_t *memory_block){
    return memory_block->start_address;
}