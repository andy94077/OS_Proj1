#ifndef _INCLUDED_SYSCALL_H_
#define _INCLUDED_SYSCALL_H_

#include <errno.h>
#include <sys/syscall.h>
#include <time.h>
#define PRINTK 333
#define GETNSTIMEOFDAY 334

inline long int printk(const char *s) {
    return syscall(PRINTK, s);
}

inline struct timespec getnstimeofday() {
    struct timespec ts;
    if (syscall(GETNSTIMEOFDAY, &ts) < 0) {
        perror("error when calling getnstimeofday");
        exit(2);
    }
    return ts;
}

#endif