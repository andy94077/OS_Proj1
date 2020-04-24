#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"
#include "scheduler.h"
#include "syscall.h"

int main() {
    enum policy p;
    char policy[8];
    scanf("%s", policy);
    switch (policy[0]) {
        case 'F':
            p = FIFO;
            break;
        case 'R':
            p = RR;
            break;
        case 'S':
            p = SJF;
            break;
        case 'P':
            p = PSJF;
            break;
        default:
            perror("Unknown policy");
            exit(-1);
    }

    int proc_n;
    scanf("%d", &proc_n);
    Process *processes = (Process *)malloc(proc_n * sizeof(*processes));

    for (int i = 0; i < proc_n; i++) {
        scanf("%s%d%d", processes[i].name, &processes[i].arrive_time, &processes[i].exec_time);
        processes[i].pid = -1;
    }

    scheduling(processes, proc_n, p);

    free(processes);
    return 0;
}