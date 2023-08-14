/* processQueue.c
*
* Created by Ke Liao (keliao@student.unimelb.edu.au) 09/04/2022
*
* This module contain functions for the construction of queue for the
* processes(which includes the struct for process). As well as different
* functions for interactions with the queue.
* 
* There are three types of queue for the purpose of process management:
* ready_queue: contains queue of ready processes.
* input_queue: process arrived but yet to allocate memory. 
* not_ready_queue: contains processes yet to arrive.
* 
* However, in case of infite memory, the process pretty much 
* move out of input queue immediately
*
* Note: the queue data structure was a modified linked list from linkedList.c 
* I've done for Assignment two of Algorithms and data structures.
* 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include "processQueue.h"
#include "simulatedMem.h"
#include "usefulConsts.h"


// Struct for storing process information
struct process_info{
    int arrive_time, time_remaining, mem_req, service_time;
    int to_process_fd, from_process_fd;
    pid_t process_pid;
    mem_block_t *mem_allocated;  // memory allocated to process
    char name[9]; // max len of 8 + '/0'
};

// Define the node struct of the queue
struct queue_node{
    process_info_t *process;    
    queue_node_t *next;
};

// Define the linked queue
struct queue{
    queue_node_t *front;
    queue_node_t *rear;
    int num_processes;
};


/* Create empty queue and return pointer associated with said queue*/
queue_t *empty_queue_create(){
    queue_t *queue;
    queue = malloc(sizeof(*queue));
    assert(queue != NULL);
    queue->front = queue->rear = NULL;
    queue->num_processes = 0;
    return queue;
}


/* Free the queue and the processes stored by the queue.*/
void queue_free(queue_t *queue){
    assert(queue != NULL);
    free_queue_nodes(queue->front);
    free(queue);
}


/*Free the node and nodes pointed to by the node of the queue*/
void free_queue_nodes(queue_node_t *queue_node){
    if(queue_node==NULL){
        return;  // No need to free if node already NULL
    }else if (queue_node->next != NULL){
        free_queue_nodes(queue_node->next);
    }
    process_info_free(queue_node->process);
    
    free(queue_node);
}


/* Add queue node to the ready queue. SJF style*/
void sjf_add_queue_node(queue_t *ready_queue, queue_node_t *process_node){

    assert(ready_queue != NULL);
    // Initialize
    queue_node_t *prev_node = NULL;  // front node has no prev nodes
    queue_node_t *curr_node = ready_queue->front; 

    // No need for SJF when adding to empty queue
    if (curr_node == NULL){
        add_queue_node(ready_queue, process_node);
        return;
    }

    // Add node, ensuring SJF
    int hasAdded = FALSE;
    while (hasAdded == FALSE){

        int curr_cmp = process_cmp(process_node->process, curr_node->process);
        if (curr_cmp <= SMALLER_THAN){

            // Insert node to queue
            if (prev_node == NULL){
                
                // Adding to front of Queue(if scheduled before front node)
                process_node->next = curr_node;
                ready_queue->front = process_node;
            }else{
                
                // Insert in middle of queue
                process_node->next = curr_node;
                prev_node->next = process_node;
            }
            hasAdded = TRUE;
            ready_queue->num_processes += 1;
        }else{

            // If at end, insert to end. Else ontinue move through queue.
            if(curr_node->next == NULL){
                add_queue_node(ready_queue, process_node);
                hasAdded = TRUE;
            }else{
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
        }
    }   
}


/* Get the process info of a queue node*/
process_info_t *get_process_info(queue_node_t *node){
    return node->process;
}


/* Pop and return the front node of queue*/
queue_node_t *pop_front_queue(queue_t *queue){
    assert(queue != NULL);
    queue_node_t *front_node = queue->front;
    assert(front_node != NULL);
    
    // Change queue front & maybe rear after poping front node
    if (front_node == queue->rear){

        // In the event that there is only one node in queue
        queue->front = queue->rear = NULL;
    }else{
        queue->front = front_node->next;
    }
    queue->num_processes -= 1;
    
    return front_node;
}


/* Pop and free the front node as well as return the process for execution*/
process_info_t *pop_process(queue_t *ready_queue){
    queue_node_t *front_node = pop_front_queue(ready_queue);
    process_info_t *curr_process = front_node->process;
    free(front_node);
    return curr_process;
}

/* Get the node at the front of queue*/
queue_node_t *get_front_node(queue_t *queue){
    return queue->front;
}


/* Add process to end of queue*/
void add_process(queue_t *queue, process_info_t *process){
    
    // Creating the new entry
    assert(queue != NULL);
    assert(process != NULL);
    queue_node_t *new_entry;
    new_entry = malloc(sizeof(*new_entry)); 
    assert(new_entry !=NULL);
    new_entry->process = process;
    new_entry->next = NULL;

    // Add the node
    add_queue_node(queue, new_entry);
}


/* Add the inputted queue node to end of queue*/
void add_queue_node(queue_t *queue, queue_node_t *queue_node){
    
    // make sure the node & queue exists
    assert(queue != NULL);
    assert(queue_node != NULL);

    // Inserting the new entry
    queue_node->next = NULL;
    if (queue->rear == NULL){
        
        // For inserting the first entry
        queue->front = queue->rear = queue_node;

    }else{
        queue->rear->next = queue_node;
        queue->rear = queue_node;
    }

    queue->num_processes += 1;
}


/* Assign allocated memory to process */
void assign_allocated_mem(process_info_t *process, mem_block_t *memory){
    process->mem_allocated = memory;
}


/* Read a line of the file contain processes. Returns the pointer
containing struct with process information. Returns NULL if reading fails*/
process_info_t *process_info_read(FILE *process_file){
    process_info_t *process = NULL;
    
    char *input_process = NULL;  // query inputs
    size_t input_len = 0;

    // Read the information from the line of the input file
    if (getline(&input_process, &input_len, process_file) != EOF){
        process = malloc(sizeof(*process));
        assert(process != NULL);
        sscanf(input_process, "%d %s %d %d", &(process->arrive_time), 
               process->name, &(process->service_time), &(process->mem_req));
        process->time_remaining = process->service_time;
        process->mem_allocated = NULL;    
        process->process_pid = 0;
        process->from_process_fd = process->to_process_fd = 0;
    }

    free(input_process);
    return process; 
}


/* Compare the two processes. Earlier processes to be 
scheduled are defined as smaller*/
int process_cmp(process_info_t *process1, process_info_t *process2){

    if (process1->service_time < process2->service_time){
        return SMALLER_THAN;
    }else if(process1->service_time> process2->service_time){
        return GREATER_THAN;
    }else{

        // Choose earliest arrival if tie with service time
        if (process1->arrive_time < process2->arrive_time){
            return SMALLER_THAN;
        }else if(process1->arrive_time > process2->arrive_time){
            return GREATER_THAN;
        }else{

            // Choose base on name if all other variables are equal
            return strcmp(process1->name, process2->name);
        }

        
    }
}


/* reduce process time_remaining by quantum*/
void decrement_remaining_time(process_info_t *process, int quantum_length){
    assert(process != NULL);
    process->time_remaining = process->time_remaining - quantum_length;

    // To fit the reality of no negative time
    if (process->time_remaining < 0){
        process->time_remaining = 0; 
    }

}


/* Free the struct*/
void process_info_free(process_info_t *process){
    free(process);
}


/* Get the length of the queue*/
int get_queue_len(queue_t *queue){
    assert(queue != NULL);
    return queue->num_processes;
}


/* get time remaining of the process*/
int get_time_remaining(process_info_t *process){
    assert(process != NULL);
    return process->time_remaining;
}


/* Get the name of the processes*/
char *get_process_name(process_info_t *process){
    assert(process != NULL);
    return process->name;
}


/* Get associated memory block of a process*/
mem_block_t *get_mem_block(process_info_t *process){
    assert(process != NULL);
    return process->mem_allocated;
}


/* Get the arrival time of a process*/
int get_arrival_time(process_info_t *process){
    assert(process != NULL);
    return process->arrive_time;
}


/* get the service time of process*/
int get_service_time(process_info_t *process){
    assert(process != NULL);
    return process->service_time;
}


/* Get memory requirement of process*/
int get_mem_req(process_info_t *process){
    assert(process != NULL);
    return process->mem_req;
}


/* Get pid associated with process*/
pid_t get_pid(process_info_t *process){
    return process->process_pid;
}


/* Set the pid associated with the process */
void set_pid(process_info_t *process, pid_t pid){
    process->process_pid = pid;
}


/* Get file descriptor for sending info to process*/
int get_to_process_fd(process_info_t *process){
    return process->to_process_fd;
}


/* Set file descriptor for sending info to process */
void set_to_process_fd(process_info_t *process, int fd){
    process->to_process_fd = fd;
}


/* Get file desc for receiving info from process*/
int get_from_process_fd(process_info_t *process){
    return process->from_process_fd;
}


/* Set file descriptor for receiving info from process*/
void set_from_process_fd(process_info_t *process, int fd){
    process->from_process_fd = fd;
}