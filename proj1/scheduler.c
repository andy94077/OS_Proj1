#define _GNU_SOURCE
#include "scheduler.h"

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"

/* get the index of the next process to execute*/
int next_process(const Process *proc, int proc_n, int running_i, enum policy p, int RR_runnning_time, int cur_time) {
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
            if (running_i == -1 || RR_runnning_time >= 500) {
                for (int i = 1; i <= proc_n; i++)  // access the next element first
                    if (proc[(running_i + i) % proc_n].exec_time > 0 && cur_time >= proc[(running_i + i) % proc_n].arrive_time)
                        return (running_i + i) % proc_n;
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

int scheduling(Process *proc, int proc_n, enum policy p) {
    process_assign_cpu(getpid(), PARENT_CPU);

    qsort(proc, proc_n, sizeof(*proc), process_cmp);

    int running_proc_n = proc_n, running_i = -1, RR_runing_time = 0, cur_time = 0;
    while (running_proc_n) {
        // start the process
        for (int i = 0; i < proc_n && cur_time <= proc[i].arrive_time; i++)
            if (proc[i].pid < 0 && proc[i].exec_time > 0)
                process_run(&proc[i]);

        if (proc[running_i].exec_time <= 0) {  // process ends
            waitpid(proc[running_i].pid, NULL, 0);
            running_i = -1;
            RR_runing_time = 0;
            running_proc_n--;
        }

        int next_i = next_process(proc, proc_n, running_i, p, RR_runing_time, cur_time);
        if (next_i != -1 && next_i != running_i) {  // a process is ready to content switch
            process_block(proc[running_i].pid);
            process_wakeup(proc[next_i].pid);
        }
        if (RR_runing_time >= 500)
            RR_runing_time = 0;
        running_i = next_i;

        UNIT_TEST();
        if (running_i != -1)
            proc[running_i].exec_time--, RR_runing_time++;
        cur_time++;
    }

    return 0;
}