#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage int sys_my_printk(const char *s){
	return printk(s);	
}

asmlinkage int sys_my_getnstimeofday(struct timespec *ts){
	getnstimeofday(ts);
	return 0;
}
