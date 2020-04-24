#ifndef _INCLUDED_PROCESS_H_
#define _INCLUDED_PROCESS_H_

#include <sys/types.h>

#define PARENT_CPU 0
#define CHILD_CPU 1

/* Running one unit time */
#define UNIT_TEST()                     \
    {                                   \
        volatile unsigned long i;       \
        for (i = 0; i < 1000000UL; i++) \
            ;                           \
    }

typedef struct Process {
    pid_t pid;
    char name[32];
    int arrive_time, exec_time;
} Process;

int process_cmp(const void *a, const void *b);

// assign process to the specific core
int process_assign_cpu(pid_t pid, unsigned int core);

// execute the process and return pid
pid_t process_run(Process *proc);

// set very low priority to the process
int process_block(pid_t pid);

// set high priority to the process
int process_wakeup(pid_t pid);

#endif
