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
    if (syscall(GETNSTIMEOFDAY, &ts) < 0) {
        perror("error when calling getnstimeofday");
        exit(2);
    }
    return ts;
}
