#ifndef _processQueue_H_
#define _processQueue_H_
#include <stdio.h>
#include <sys/types.h>
#include "simulatedMem.h"


typedef struct process_info process_info_t;
typedef struct queue_node queue_node_t;
typedef struct queue queue_t;

queue_t *empty_queue_create();
void queue_free(queue_t *queue);
void free_queue_nodes(queue_node_t *queue_node);
void sjf_add_queue_node(queue_t *ready_queue, queue_node_t *process_node);
void assign_allocated_mem(process_info_t *process, mem_block_t *memory);
queue_node_t *pop_front_queue(queue_t *queue);
process_info_t *get_process_info(queue_node_t *node);
process_info_t *process_info_read(FILE *process_file);
int process_cmp(process_info_t *process1, process_info_t *process2);
process_info_t *pop_process(queue_t *ready_queue);
void add_process(queue_t *queue, process_info_t *process);
void add_queue_node(queue_t *queue, queue_node_t *queue_node);
queue_node_t *get_front_node(queue_t *queue);
void process_info_free(process_info_t *process);
int get_queue_len(queue_t *queue);
void decrement_remaining_time(process_info_t *process, int quantum_length);
int get_time_remaining(process_info_t *process);
char *get_process_name(process_info_t *process);
mem_block_t *get_mem_block(process_info_t *process);
int get_arrival_time(process_info_t *process);
int get_service_time(process_info_t *process);
int get_mem_req(process_info_t *process);
pid_t get_pid(process_info_t *process);
void set_pid(process_info_t *process, pid_t pid);
int get_to_process_fd(process_info_t *process);
void set_to_process_fd(process_info_t *process, int fd);
int get_from_process_fd(process_info_t *process);
void set_from_process_fd(process_info_t *process, int fd);
#endif