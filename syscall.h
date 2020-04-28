#ifndef _INCLUDED_SYSCALL_H_
#define _INCLUDED_SYSCALL_H_

#include <time.h>

long printk(const char *s);

struct timespec getnstimeofday();

#endif