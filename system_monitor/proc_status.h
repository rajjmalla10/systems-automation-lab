#ifndef PROC_STATUS_H
#define PROC_STATUS_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>  
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else 
#define DEBUG_PRINT(fmt, ...)
#endif

struct proc_info{
	pid_t pid;
	pid_t ppid;
	long vmrss;
    long vmsize_KB;
	
	char name[256]; // process name 
	
	char state;
	int num_threads;
	int nice_value;
	long priority; //new: process priority 
	
	long utime;	// New: user cpu time in ticks
	long stime;	// new: system cpu time in ticks
	long cutime;// new: child user time
	long cstime; // new: child system time
	long itrealvalue;
	long starttime;	//new: when process started
	
	float cpu_percentage;  // current cpu usage 
	
	
	unsigned long last_cpu_ticks; //for tracking changes previous (utime + stime)
	struct timeval last_cpu_checks;
	
	};

typedef struct proc_node{
	struct proc_info info; //data
	struct proc_node *next; //next node
	struct proc_node *prev; // previous node
	int active;
} proc_node_t;	




	
// Read procs status from /proc/[pid]/status
//parameters: 
	// pid;- process ID to read (0 for current process)
	//info:- pointer to struct proc_info to fill 
	
	// returns: 0 on sucess 
	// -1 on error (check errno)
	
int read_proc_status(pid_t pid, struct proc_info *info);
int read_proc_stat(pid_t pid, struct proc_info *info);
int read_proc_full_info(pid_t pid, struct proc_info *info);
int read_proc_self_full_info(struct proc_info *info);	
void add_node(proc_node_t **head, struct proc_info info);
void to_remove(proc_node_t **head, proc_node_t *to_remove);
float calculate_cpu_percentage(proc_node_t *head);
#endif
