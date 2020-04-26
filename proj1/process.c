#define _GNU_SOURCE
#include "process.h"

#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "syscall.h"

int process_cmp(const void *a, const void *b) {
    return ((Process *)a)->arrive_time - ((Process *)b)->arrive_time;
}

int process_assign_cpu(pid_t pid, unsigned int core) {
    if (core > sizeof(cpu_set_t)) {
        perror("invalid core index");
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);

    if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
        perror("sched_setaffinity error");
        exit(1);
    }

    return 0;
}

pid_t process_run(Process *proc) {
    pid_t pid = fork();
    if (pid == 0) {  // child
        process_assign_cpu(getpid(), CHILD_CPU);
        printf("%s %d\n", proc->name, getpid());
        process_block(getpid());
        struct timespec ts_start = getnstimeofday();
#ifdef DEBUG
        printf("[%ld.%09ld] process %s, pid %d run\n", ts_start.tv_sec, ts_start.tv_nsec / (int)1e6, proc->name, getpid());
#endif

        for (int i = 0; i < proc->exec_time; i++)
            UNIT_TEST();

        struct timespec ts_end = getnstimeofday();
        char msg[256];
#ifdef DEBUG
        sprintf(msg, "[Project1] %d %ld.%09ld %ld.%09ld %lld\n", getpid(), ts_start.tv_sec, ts_start.tv_nsec, ts_end.tv_sec, ts_end.tv_nsec, (long long)ts_end.tv_sec * (long long)1e9 + (long long)ts_end.tv_nsec - (long long)ts_start.tv_sec * (long long)1e9 - (long long)ts_start.tv_nsec);
#else
        sprintf(msg, "[Project1] %d %ld.%09ld %ld.%09ld\n", getpid(), ts_start.tv_sec, ts_start.tv_nsec, ts_end.tv_sec, ts_end.tv_nsec);
#endif
        printk(msg);
        exit(0);
    } else  // parent
        proc->pid = pid;
    return pid;
}

// set very low priority to the process
int process_block(pid_t pid) {
    struct sched_param param;
    // SCHED_IDLE should set priority to 0
    param.sched_priority = 0;

    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
    if (ret < 0) {
        char s[256];
        sprintf(s, "\npid: %d block sched_setscheduler error", pid);
        perror(s);
        exit(3);
    }
    return ret;
}

// set high priority to the process
int process_wakeup(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 9;

    int ret = sched_setscheduler(pid, SCHED_FIFO, &param);
    if (ret < 0) {
        char s[256];
        sprintf(s, "\npid: %d wakeup sched_setscheduler error", pid);
        perror(s);
        exit(4);
    }
    return ret;
}
