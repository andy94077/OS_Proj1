#define _GNU_SOURCE
#include "scheduler.h"

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"
#include "syscall.h"

int next_process(const Process *proc, int proc_n, int running_i, enum policy p, int RR_runnning_time, int cur_time, int process_dead, Process **RR_proc_queue) {
    switch (p) {
        case FIFO:
            if (running_i == -1) {
                for (int i = 0; i < proc_n && cur_time >= proc[i].arrive_time; i++)
                    if (proc[i].exec_time > 0)
                        return i;
                return -1;  // no process is ready
            }
            return running_i;
            break;

        case RR:
            if (running_i == -1 || RR_runnning_time >= 500 || process_dead) {
                for (int i = 0; i < proc_n; i++) {
                    if (RR_proc_queue[i]->exec_time > 0 && cur_time >= RR_proc_queue[i]->arrive_time) {
                        Process *tmp = RR_proc_queue[i];
                        
                        /*
                        *   each of ready processes move forward
                        */
                        int j = i + 1;
                        for (; j < proc_n && cur_time >= RR_proc_queue[j]->arrive_time; j++)
                            RR_proc_queue[j - 1] = RR_proc_queue[j];
                        
                        // push the running process to the back
                        RR_proc_queue[j - 1] = tmp;
                        return RR_proc_queue[j - 1] - proc; // return the index of the process
                    }
                }
                return -1;  // no process is ready
            }
            return running_i;
            break;

        case SJF:
            if (running_i == -1) {
                int shortest_job_i = -1;
                for (int i = 0; i < proc_n && cur_time >= proc[i].arrive_time; i++)
                    if (proc[i].exec_time > 0 && (shortest_job_i < 0 || proc[shortest_job_i].exec_time > proc[i].exec_time))
                        shortest_job_i = i;
                return shortest_job_i;  // maybe no process is ready
            }
            return running_i;
            break;

        case PSJF:;  // add semi-column because declaring variables in switch cases is prohibited in C
            int shortest_job_i = -1;
            for (int i = 0; i < proc_n && cur_time >= proc[i].arrive_time; i++)
                if (proc[i].exec_time > 0 && (shortest_job_i < 0 || proc[shortest_job_i].exec_time > proc[i].exec_time))
                    shortest_job_i = i;
            return shortest_job_i;  // maybe no process is ready
            break;

        default:
            break;
    }
    return -1;
}

void scheduling(Process *proc, int proc_n, enum policy p) {
    process_wakeup(getpid());
    process_assign_cpu(getpid(), PARENT_CPU);

    Process **RR_proc_queue = (Process **)malloc(proc_n * sizeof(Process *));
    for (int i = 0; i < proc_n;i++)
        RR_proc_queue[i] = &proc[i];
    
    int running_proc_n = proc_n, running_i = -1, RR_runing_time = 0, cur_time = 0;
    while (running_proc_n) {
        // start the process
        for (int i = 0; i < proc_n && cur_time >= proc[i].arrive_time; i++) {
            if (proc[i].pid < 0 && proc[i].exec_time > 0) {
                process_run(&proc[i]);
                process_block(proc[i].pid);
#ifdef DEBUG
                fprintf(stderr, "cur_time: %d, create and block %s, pid %d\n", cur_time, proc[i].name, proc[i].pid);
#endif
            }
        }

        int process_dead = 0;
        if (running_i >= 0 && proc[running_i].exec_time <= 0) {  // process ends
            waitpid(proc[running_i].pid, NULL, 0);
            // RR have to keep the original running_i.
            // However, we still have to notify next_process that we have to change process
            if (p != RR)
                running_i = -1;
            process_dead = 1;

            RR_runing_time = 0;
            running_proc_n--;
        }

        int next_i = next_process(proc, proc_n, running_i, p, RR_runing_time, cur_time, process_dead, RR_proc_queue);
        if (next_i != -1 && next_i != running_i) {  // a process is ready to content switch
            if (running_i >= 0 && !process_dead) {
                process_block(proc[running_i].pid);
#ifdef DEBUG
                fprintf(stderr, "cur_time: %d, block %s, pid %d\n", cur_time, proc[running_i].name, proc[running_i].pid);
#endif
            }

#ifdef DEBUG
            fprintf(stderr, "cur_time: %d, running_i: %d, wakeup %s, pid %d\n", cur_time, running_i, proc[next_i].name, proc[next_i].pid);
#endif
            process_wakeup(proc[next_i].pid);
            process_dead = 0;
        }
        if (RR_runing_time >= 500)
            RR_runing_time = 0;
        running_i = next_i;

        UNIT_TEST();
        if (running_i != -1)
            proc[running_i].exec_time--, RR_runing_time++;
        cur_time++;
    }
    free(RR_proc_queue);
}