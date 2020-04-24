#ifndef _INCLUDED_SYSCALL_H_
#define _INCLUDED_SYSCALL_H_

#include <time.h>

inline long printk(const char *s);

inline struct timespec getnstimeofday();

#endif