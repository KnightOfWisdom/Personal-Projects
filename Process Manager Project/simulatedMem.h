#ifndef _SIMULATED_MEM_H_
#define _SIMULATED_MEM_H_
#include <stdio.h>

typedef struct mem_block mem_block_t;
typedef struct sim_mem sim_mem_t;

sim_mem_t *new_sim_mem(int mem_size);
mem_block_t *new_mem_block(int start, int length, int isFree);
void memory_to_hole(mem_block_t *mem_block, sim_mem_t *sim_mem);
void free_memory_merge(mem_block_t *mem_block, sim_mem_t *sim_mem);
mem_block_t *best_fit_mem_allocate(int mem_req, sim_mem_t *simulated_mem);
void free_simulated_mem(sim_mem_t *simulated_mem);
void mem_blocks_free(mem_block_t *mem_block);
int get_start_address(mem_block_t *memory_block);

#endif