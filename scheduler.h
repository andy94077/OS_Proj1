#ifndef _INCLUDED_SCHEDULER_H_
#define _INCLUDED_SCHEDULER_H_

#include "process.h"

// FIFO, RR, SJF, PSJF
enum policy {
    FIFO,
    RR,
    SJF,
    PSJF
};

void scheduling(Process *proc, int proc_n, enum policy p);

#endif