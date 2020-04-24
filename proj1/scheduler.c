#define _GNU_SOURCE
#include "scheduler.h"

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"

int scheduling(Process *proc, int proc_n, enum policy p) {
    return 0;
}