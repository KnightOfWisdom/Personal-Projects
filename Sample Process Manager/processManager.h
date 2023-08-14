#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_
#include <stdio.h>
#include <stdint.h>

#define BEST_FIT_MEM_LIMIT 2048  // Limit of available memory simulated
#define START_TIME 0
#define READ_END 0  // Read end pipeline
#define WRITE_END 1  // Write end pipeline
#define SHA_LEN 64 // length of SHA output of program

void process_manager(FILE *process_file, int is_sjf, 
                     int mem_is_finite, int quantum);
void completion_handler(process_info_t *process, int time, int proc_remaining, 
                        sim_mem_t *simulated_mem);
void arrival_handler(queue_t *not_ready, queue_t *input, int time);
void memory_alloc_handler(queue_t *input, queue_t *ready, int is_sjf,
                          sim_mem_t *sim_mem, int mem_is_finite, int time);
process_info_t *execution_handler(process_info_t *process, int time, 
                                  queue_t *ready, char* proc_path);
void stat_print(double total_turnaround, int num_finished,  
                double total_overhead, double max_overhead, int time);
void sucessful_mem_alloc_print(process_info_t *process, int time);
void execute_process(process_info_t *process, int time, char *process_path);
void continue_process(process_info_t *process, int time);
void suspend_process(process_info_t *process, int time);
void terminate_process(process_info_t *process, int time);
void process_sha_print(process_info_t *process, int time, char* sha);
void new_exe_process_info_print(process_info_t *process, int time);
uint8_t *to_big_endian(int num);
#endif