#define _GNU_SOURCE
#include "syscall.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#define PRINTK 333
#define GETNSTIMEOFDAY 334

long printk(const char *s) {
    return syscall(PRINTK, s);
}

struct timespec getnstimeofday() {
    struct timespec ts;
    syscall(GETNSTIMEOFDAY, &ts);
    return ts;
}
