#ifndef _INCLUDED_SCHEDULING_H_
#define _INCLUDED_SCHEDULING_H_

#include "process.h"

enum policy {
    FIFO,
    RR,
    SJF,
    PSJF
};

int scheduling(Process *proc, int proc_n, enum policy policy);

#endif