/**ProcessManager.c
 * 
 * Created by Ke Liao (keliao@student.unimelb.edu.au) 12/04/2022
 * 
 * This module contain functions that allows the creation and running of the 
 * process manager. The process creation and memory allocation are simulated. 
 * Also contain functions for output information relating to the scheduler to 
 * stdout.
 * 
 * Note: this process Manager only simulate best fit in even of finite memory
 * and scheduler is either SJF or Round Robin.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h> 
#include <unistd.h>
#include "processQueue.h"
#include "simulatedMem.h"
#include "usefulConsts.h"
#include "processManager.h"


void process_manager(FILE *process_file, int is_sjf, 
                     int is_finite_mem, int quantum){

    int time = START_TIME;
    int num_left = 0;  // Num process left that could be executed
    int num_finished = 0; // Num processes that finish
    double tot_turnaround = 0;
    double tot_overhead = 0; // for average calc
    double max_overhead = 0;
    char proc_path[] = "./process";   // path to the process for simulation

    // Set up the queues
    queue_t *ready, *input, *not_ready;
    ready = empty_queue_create();
    input = empty_queue_create();
    not_ready = empty_queue_create();

    // Create simulated memory(in event finite mem)
    sim_mem_t *sim_mem = NULL;
    if (is_finite_mem){
        sim_mem = new_sim_mem(BEST_FIT_MEM_LIMIT);
    }
    
    // Read & record processes in the input file
    process_info_t *process;
    while ((process = process_info_read(process_file)) != NULL){
        add_process(not_ready, process);
        num_left += 1;
    }

    // Now perform simulation
    process_info_t *running_proc = NULL;
    while ((num_left > 0) || (running_proc != NULL)){

        // Step 1: check for process completion
        int proc_remaining = get_queue_len(ready) + get_queue_len(input);
        if (running_proc != NULL){
            int time_left = get_time_remaining(running_proc);
            if (time_left <= 0){
                
                // Keep track statistics
                int proc_start_time = get_arrival_time(running_proc);
                double curr_turnaround = time - proc_start_time;
                tot_turnaround += curr_turnaround;
                int service_time = get_service_time(running_proc);
                double curr_overhead = curr_turnaround/service_time;
                if (curr_overhead > max_overhead){
                    max_overhead = curr_overhead;
                }
                tot_overhead += curr_overhead;
                
                // Handle & keep track of process completion
                completion_handler(running_proc, time, proc_remaining, 
                                   sim_mem);
                num_left -= 1;
                num_finished += 1;
                running_proc = NULL;
            }
        }

        // Step 2: Check which processes have arrived
        arrival_handler(not_ready, input, time);

        // Step 3: Memory allocation
        memory_alloc_handler(input, ready, is_sjf, sim_mem, 
                             is_finite_mem, time);

        // Round Robin, Move executing process back of line
        int num_ready = get_queue_len(ready);
        if (is_sjf == FALSE){
            if (running_proc != NULL && num_ready >= 1){
                add_process(ready, running_proc);
                suspend_process(running_proc, time);
                running_proc = NULL;
            }    
        }

        // Step 4: check for potential process execution
        running_proc = execution_handler(running_proc, time, ready, proc_path);

        // Increment time
        if (num_left > 0){
            time += quantum;
            if (running_proc != NULL){
                decrement_remaining_time(running_proc, quantum);
            }
        }
    }

    stat_print(tot_turnaround, num_finished, tot_overhead, max_overhead, time);

    // Free everything
    queue_free(ready);
    queue_free(input);
    queue_free(not_ready);
    if (sim_mem != NULL){
        free_simulated_mem(sim_mem);
    }
}


/* Handle the completion of process. */
void completion_handler(process_info_t *process, int time, int proc_remaining, 
                        sim_mem_t *simulated_mem){
    
    // Terminate & get necessary output of process
    char *name = get_process_name(process);
    printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n",
                   time, name, proc_remaining);
    terminate_process(process, time);

    // Free the associated memory if needed
    mem_block_t *assigned_mem = get_mem_block(process);
    if (assigned_mem != NULL){
        memory_to_hole(assigned_mem, simulated_mem);
    }
    process_info_free(process);
    process = NULL;
}


/* Handle arrival of processes*/
void arrival_handler(queue_t *not_ready, queue_t *input, int time){
    queue_node_t *curr_node = get_front_node(not_ready);
    while (curr_node != NULL){
        process_info_t *process = get_process_info(curr_node);
        int process_arrival = get_arrival_time(process);
        if (process_arrival <= time){
            pop_front_queue(not_ready);
            add_queue_node(input, curr_node);
            curr_node = get_front_node(not_ready);
        }else{

            // Processes in not_ready already in order(no need check other)
            break;
        }
    }
}


/* Handle allocation memory and move to ready queue if allocation successful*/
void memory_alloc_handler(queue_t *input, queue_t *ready, int is_sjf,
                          sim_mem_t *sim_mem, int is_finite_mem, int time){
    
    if (is_finite_mem){
        // Best fit allocation
        int num_needed_allocation = get_queue_len(input);
        for (int i = 0; i < num_needed_allocation; i++){

            // Try to allocate mem to current front of input queue
            queue_node_t *curr = pop_front_queue(input);
            process_info_t *curr_process = get_process_info(curr);
            int mem_req = get_mem_req(curr_process);
            mem_block_t *alloc_mem = best_fit_mem_allocate(mem_req, sim_mem);


            if (alloc_mem != NULL){

                // Move to ready queue based on scheduling alg
                assign_allocated_mem(curr_process, alloc_mem);
                if (is_sjf){
                    sjf_add_queue_node(ready, curr);
                }else{
                    add_queue_node(ready, curr);
                }
                sucessful_mem_alloc_print(curr_process, time);
            }else{

                // The loop won't reach nodes sent to back of queue
                add_queue_node(input, curr);
            }
        }

    }else{

        // No real need for memory allocation if infinite mem
        queue_node_t *curr = get_front_node(input);
        while (curr != NULL){
            pop_front_queue(input);
            if (is_sjf){
                sjf_add_queue_node(ready, curr);
            }else{
                add_queue_node(ready, curr);
            }
            curr = get_front_node(input);
        }
    }

}


/* Handler execution of processes. Return address of current running process*/
process_info_t *execution_handler(process_info_t *process, int time, 
                                  queue_t *ready, char* proc_path){

    process_info_t *running_proc = process;
    int num_ready_proc = get_queue_len(ready);
    if ((running_proc == NULL) && (num_ready_proc > 0)){
        running_proc = pop_process(ready);
        new_exe_process_info_print(running_proc, time);

        pid_t curr_pid = get_pid(running_proc);
        if (curr_pid == 0){
            execute_process(running_proc, time, proc_path);
        }else{

            // curr_pid != 0 means resumption of a suspended process
            continue_process(running_proc, time);
        }
    }else if(running_proc != NULL){
        
        // Continue process
        continue_process(running_proc, time);
    }
    return running_proc;
}


/* print the performance statistics */
void stat_print(double total_turnaround, int num_finished,  
                double total_overhead, double max_overhead, int time){
    double ave_turn_around = ceil(total_turnaround/num_finished);
    double ave_overhead = total_overhead/num_finished;

    // round to 2 dec.
    ave_overhead = round(ave_overhead * 100) / 100; 
    max_overhead = round(max_overhead * 100) / 100;

    // Print out time results   
    printf("Turnaround time %.0f\n", ave_turn_around);
    printf("Time overhead %.2f %.2f\n",max_overhead, ave_overhead);
    printf("Makespan %d\n", time);
}


/* Print out message for successful memory allocation*/
void sucessful_mem_alloc_print(process_info_t *process, int time){
    char *name = get_process_name(process);
    mem_block_t *process_mem = get_mem_block(process);
    int start_loc = get_start_address(process_mem);
    printf("%d,READY,process_name=%s,assigned_at=%d\n", time, name, start_loc);
}


/* Run the process located at information from process_path*/
void execute_process(process_info_t *process, int time, char *process_path){
    int to_process_fd[2], from_process_fd[2];
    pipe(to_process_fd);
    pipe(from_process_fd);
    pid_t process_pid = fork();

    if (process_pid == 0){

        // Set up pipeline and execution of child process
        close(to_process_fd[WRITE_END]);
        close(from_process_fd[READ_END]);
        dup2(from_process_fd[WRITE_END], STDOUT_FILENO);
        dup2(to_process_fd[READ_END], STDIN_FILENO);
        char *name = get_process_name(process);
        char *process_arg[] ={process_path, name, NULL};
        execv(process_path, process_arg);
    }else{

        // Set up pipeline for parent and send simulation time to child
        close(to_process_fd[READ_END]);
        close(from_process_fd[WRITE_END]);
        set_to_process_fd(process, to_process_fd[WRITE_END]);
        set_from_process_fd(process, from_process_fd[READ_END]);
        set_pid(process, process_pid);
        uint8_t *big_endian_time = to_big_endian(time);
        write(to_process_fd[WRITE_END], big_endian_time, BYTES_IN_INT);

        // Verify 1 byte output same as least significant byte sent
        uint8_t std_out_proc[1];
        read(from_process_fd[READ_END], std_out_proc, sizeof(std_out_proc));
        assert(std_out_proc[0] == big_endian_time[3]);
        
    }
}


/* Continue/resume the process*/
void continue_process(process_info_t *process, int time){
    pid_t process_pid = get_pid(process);
    uint8_t *big_endian_time = to_big_endian(time);
    int to_process_fd = get_to_process_fd(process);
    write(to_process_fd, big_endian_time, BYTES_IN_INT);
    kill(process_pid, SIGCONT);

    // Verify 1 byte output same as least significant byte sent
    int from_process_fd = get_from_process_fd(process);
    uint8_t std_out_proc[1];
    read(from_process_fd, std_out_proc, sizeof(std_out_proc));
    assert(std_out_proc[0] == big_endian_time[3]);
    
}


/* Suspend the execution of process */
void suspend_process(process_info_t *process, int time){
    pid_t process_pid = get_pid(process);
    uint8_t *big_endian_time = to_big_endian(time);
    int to_process_fd = get_to_process_fd(process);
    write(to_process_fd, big_endian_time, BYTES_IN_INT);
    kill(process_pid, SIGTSTP);
    int proc_status = 0;
    waitpid(process_pid, &proc_status, WUNTRACED);
    while(TRUE){
        if (WIFSTOPPED(proc_status)){
            break;
        }
    }
}


/* Terminate the execution of process*/
void terminate_process(process_info_t *process, int time){
    pid_t process_pid = get_pid(process);
    uint8_t *big_endian_time = to_big_endian(time);
    int to_process_fd = get_to_process_fd(process);
    write(to_process_fd, big_endian_time, BYTES_IN_INT);
    kill(process_pid, SIGTERM);
    int proc_status = 0;
    waitpid(process_pid, &proc_status, WUNTRACED);
    while(TRUE){
        if (WIFEXITED(proc_status)){
            break;
        }
    }

    // Deal with and print out sha output
    char sha_buffer[SHA_LEN + 1];
    int from_process_fd = get_from_process_fd(process);
    read(from_process_fd, sha_buffer, sizeof(sha_buffer));
    sha_buffer[SHA_LEN] = '\0';
    process_sha_print(process, time, sha_buffer);
}


/* Print out the Sha message for terminating program*/
void process_sha_print(process_info_t *process, int time, char* sha){
    char *name = get_process_name(process);
    printf("%d,FINISHED-PROCESS,process_name=%s,sha=%s\n", time, name, sha);
}


/* print info of newly executed process */
void new_exe_process_info_print(process_info_t *process, int time){
    assert(process != NULL);
    char *name = get_process_name(process);
    int time_remaining = get_time_remaining(process);
    printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", time, 
           name, time_remaining);
}


/* Convert an integer to a byte array of length 4 following big endian*/
uint8_t *to_big_endian(int num){
    static uint8_t byte_arr[BYTES_IN_INT];
    byte_arr[0] = (num >> 24) & 0xFF;
    byte_arr[1] = (num >> 16) & 0xFF;
    byte_arr[2] = (num >> 8) & 0xFF;
    byte_arr[3] = num & 0xFF;
    return byte_arr;
}