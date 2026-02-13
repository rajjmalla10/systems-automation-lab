#include "proc_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define READ_SIZE 256
#define CARRY_SIZE 1024

proc_node_t *index[32768] = {0};
//Buffer managment code, 
static int parse_proc_status(int fd, struct proc_info *info){
	if(fd<0){
		errno = EBADF; // bad file descriptor
		return -1;
		}
	
	ssize_t bytes_read;
	char read_buffer[READ_SIZE];
	char carry_buffer[CARRY_SIZE];
	size_t carry_len = 0;
	int found_pid = 0,  found_name = 0, found_mem = 0, found_state = 0, found_thread = 0;	
	
	while((bytes_read=read(fd,read_buffer,sizeof(read_buffer)))>0){
		
		for(ssize_t i =0; i < bytes_read && carry_len < CARRY_SIZE; i++){
			carry_buffer[carry_len++] = read_buffer[i];	
			}
			
			if(carry_len >= CARRY_SIZE && bytes_read > 0){
				fprintf(stderr,"Line too long for buffer\n");
				return -1;
				
				}
				
					
		//parse logic
		
		size_t length_tracker = 0; 
		size_t offset_tracker = 0;
		while(length_tracker<carry_len){
			
			
			
			if(carry_buffer[length_tracker] == '\n'){
				char current = carry_buffer[length_tracker];
				carry_buffer[length_tracker] = '\0';
				char* line = carry_buffer + offset_tracker;
				
				
				if((strncmp(line,"Pid:",4) == 0)){
					
					(*info).pid = atoi(line+4);
					found_pid = 1;
					
				}
				else if((strncmp(line,"PPid:",5) == 0)){
					
					(*info).ppid = atoi(line+5);
					
					
					}
				else if((strncmp(line,"VmRSS:",6) == 0)){
					
					info->vmrss = atoi(line+6);
					found_mem = 1;
					}
				else if((strncmp(line,"Name:",5) == 0)){	
					
					char* p = line + 5;
					while( *p ==' ' || *p == '\t') p++;
					size_t max_copy = sizeof((*info).name)-1;
					strncpy((*info).name,p,max_copy);
					(*info).name[max_copy] = '\0';
					found_name = 1;
					
				}
				
				else if(strncmp(line,"State:",6) == 0){
					char* p = line + 6;
					if( *p == ' ' || *p == '\t') p++;
					info->state = *p;
					found_state = 1;
					
					}
					
				else if(strncmp(line,"Threads:",8) == 0){
					info->num_threads = atoi(line + 8);
					found_thread = 1;
					}	
					
					
					
						
				
				carry_buffer[length_tracker] = current;
				
				offset_tracker = 1 + length_tracker;
				
				//early exit if we found all goal we need and clear memory as well
				
				
				if(found_name && found_pid && found_mem && found_state && found_thread ){
					size_t remaning; 
					remaning = carry_len - offset_tracker;
					if(remaning>0){
						memmove(carry_buffer,carry_buffer+offset_tracker,remaning);
						}
					carry_len = remaning;
					return 0;	
					
					}
			
				
		}
		length_tracker ++;
		
		}
		//shifting memory 
		if(offset_tracker>0){
			size_t unprocessed_bytes = carry_len - offset_tracker;
			if(unprocessed_bytes>0){
				memmove(carry_buffer,carry_buffer + offset_tracker,unprocessed_bytes);
				}
			carry_len = unprocessed_bytes;
			}
		
	}
	
	if(bytes_read==0){
		//EOF reached 
		
		if(found_name && found_pid ){
			return 0;
		}
		else{
			DEBUG_PRINT("Missing field in status file!\n");
			return -1;
			}
		}

	return -1;
}	

// stat buffer 

static int parse_proc_stat(int fd, struct proc_info *info){
	if(fd<0){
		errno = EINVAL;
		return -1;
		}
	ssize_t read_bytes;
	char buffer[1024];
	
	read_bytes = read(fd,buffer,sizeof(buffer) -1);
	if(read_bytes <= 0){
		return -1;
		}	
	buffer[read_bytes] = '\0';	
	
	char* start_point = strchr(buffer,'(');
	char* end_point = strchr(buffer,')');
	
	if(!start_point || !end_point){
		return -1; 
		}
			
	//where to start the parsing i.e the index, 
	
	char* field_parsing = end_point + 1;
	while (*field_parsing == ' ') field_parsing++;
	char state; 
	int ppid, pgrp, session, tty_nr, tpgid;
	unsigned long flags;
	unsigned long minflt, cminflt, majflt, cmajflt;
	unsigned long utime, stime; 
	long cutime, cstime; 
	long priority, nice;
	int num_threads;
	long itrealvalue;
	long starttime;
	
	int matched = sscanf(field_parsing,
    "%c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %d %ld %ld",
    &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags,
    &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime,
    &cutime, &cstime, &priority, &nice, &num_threads,&itrealvalue, &starttime);


	if(matched >= 20){
		
    
		info->priority = priority;
		info->nice_value = (int)nice;
		info->utime = (long)utime;
		info->stime = (long)stime;
		if(info->utime > 0 || info->stime > 0){
			DEBUG_PRINT("Process PID %d has CPU info: utime=%ld, stime=%ld\n", info->pid, info->utime, info->stime);
		} else {
			DEBUG_PRINT("Process PID %d: Cannot read CPU info (utime=0, stime=0)\n", info->pid);
		}
		info->cutime = cutime;
		info->cstime = cstime;
		info->starttime = starttime;
		return 0;
    }	
	
	return -1;
}

//add proc node 
void add_node(proc_node_t **head, struct proc_info info){
	proc_node_t *new_node = (proc_node_t *)malloc(sizeof(proc_node_t));
	if(new_node == NULL){
		DEBUG_PRINT("Failed to allocate memory for new process nide");
		return;
		}
	new_node->info = info;
	new_node->active = 1;
	
	new_node -> next = *head;
	new_node->prev = NULL;
	
	if(*head!=NULL){
		(*head)->prev = new_node;
		}
	*head = new_node;	
	if(info.pid >= 0 && info.pid < 32768){
		index[info.pid] = new_node;
		}
	}	

void to_remove(proc_node_t **head, proc_node_t *node_to_delete){
	if(head == NULL || *head == NULL || node_to_delete == NULL) return;
	
	// 1. if the node we are removing is the head; 
	if(*head == node_to_delete){
		*head = node_to_delete->next;
		}
	
	//2, if there's a neighbout behind us, tell them to point to our next. 
	if(node_to_delete->prev!=NULL){
		node_to_delete->prev->next = node_to_delete->next;
		}
	
	if(node_to_delete->next!=NULL){
		node_to_delete->next->prev = node_to_delete->prev;
		}	
		
	free(node_to_delete);	
	}	
	
//~ if(fd<0){
	//~ errno = EBADFD;
	//~ return -1;
	//~ }
	
//~ char buffer[READ_SIZE]; 	
//~ ssize_t bytes_read;	

//~ int field = 1;
//~ int target_field = 19;

//~ ssize_t end = -1, start = -1;

//~ while((bytes_read=read(fd,buffer,sizeof(buffer))) > 0){
	//~ for(ssize_t i = 0; i < bytes_read; i++){
		//~ if(buffer[i] == ' ' || buffer[i] == '\n'){
			//~ if(field == target_field){
				//~ end = i;
				//~ goto found;
			//~ }
			//~ field++;
			//~ if(field == target_field){
				//~ start = i + 1;
				//~ }	
		//~ }
	//~ }
	
	
//~ }
//~ found:	
//~ if(start>=0 && end > start){
	//~ char destination[26];
	//~ ssize_t length = end - start;
	
	//~ if(length >= (ssize_t)sizeof(destination)){
		//~ length = sizeof(destination) - 1;
	//~ }
	//~ memcpy(destination,buffer+start,length);
	//~ destination[length] = '\0';
	
	//~ info->nice_value = atoi(destination);
	//~ return 0;
//~ }
//~ return -1;	
//}

//READ ALL PROCESS INFO FROM BOTH STATUS AND STAT FILES

int read_proc_full_info(pid_t pid, struct proc_info *info){
	if(!info){
		errno = EINVAL;
		return -1;
	}
		
	//clear struct 
	if(pid<0){
		errno = EINVAL;
		return -1;
		}
		
		
	memset(info,0,sizeof(*info));
	
	//read basic info from status file
	
	if(read_proc_status(pid,info) != 0){
		return -1;
		}	
	//Read detailed cpu.scheduling info from the stat file
	
	if(read_proc_stat(pid,info)!= 0){
		//warning: stat might fail but we still have basic info. 
		info->utime = 0;
		info->stime = 0;
		
		}	
	
	return 0;
	}




//Read proc status




int read_proc_status(pid_t pid, struct proc_info *info){
	if(pid<=0){
		pid = getpid();
		}
	
	char path[64];
	
	snprintf(path,sizeof(path),"/proc/%d/status",(int)pid);
	int fd = open(path,O_RDONLY);
	if(fd < 0){
		
		return -1;
		}
	int result = parse_proc_status(fd,info); //
	close(fd);		
	
	return result;	
	}

//Read_proc_stat

int read_proc_stat(pid_t pid,struct proc_info *info){
	if(pid<=0){
		pid = getpid();
		}
	char buffer[64];
	snprintf(buffer,sizeof(buffer),"/proc/%d/stat",(int)pid);
	int fd = open(buffer,O_RDONLY);
	if(fd<0){
		return -1;
		}
	int result = parse_proc_stat(fd,info);
	close(fd);

	return result;				
	}	

// u time and s time into percentagem for current and previous process time,

float calculate_cpu_percentage(proc_node_t *head){
	if(head == NULL){
		DEBUG_PRINT("Head doesnt exist or is curropt!!\n");
		return 0.0f;
		}
	
	long current_ticks;
	current_ticks = head->info.utime + head->info.stime;
	
	
	
	struct timeval now;
	gettimeofday(&now,NULL);
	
	if(head->info.last_cpu_ticks == 0){
		head->info.last_cpu_ticks = current_ticks;
		head->info.last_cpu_checks = now;
		return 0.0f;
	}	
	
	if((long)current_ticks < (long)head->info.last_cpu_ticks){
        // Process was restarted, reset tracking
        head->info.last_cpu_ticks = current_ticks;
        head->info.last_cpu_checks= now;
        return 0.0f;
    }
	
	
	long tick_difference = current_ticks - head->info.last_cpu_ticks;
	double time_diff;
	//calculate time difference in seconds. 
	if(now.tv_usec < head->info.last_cpu_checks.tv_usec){
		time_diff = (now.tv_sec - head->info.last_cpu_checks.tv_sec -1) + (now.tv_usec + 1000000 - head->info.last_cpu_checks.tv_usec) / 1000000.0; 
	}else{
		time_diff = (now.tv_sec - head->info.last_cpu_checks.tv_sec ) + (now.tv_usec - head->info.last_cpu_checks.tv_usec) / 1000000.0;
		}
	if(time_diff < 0.1){
		return head->info.cpu_percentage;
		}
	
	long hz = sysconf(_SC_CLK_TCK);
	
	
	long numCores = sysconf(_SC_NPROCESSORS_ONLN);
	
	double cpu_percentage = (tick_difference / (double)hz) / time_diff * 100.0 / numCores;
		
	if(cpu_percentage < 0.0f) cpu_percentage = 0.0f;
	if(cpu_percentage > 100.0f) cpu_percentage = 100.0f;
		
		
		

	
	head->info.last_cpu_ticks = current_ticks;
	head->info.last_cpu_checks = now;
	head->info.cpu_percentage = cpu_percentage;
	
	return cpu_percentage;
	
	}	
	
int read_proc_self_full_info(struct proc_info *info){
	return read_proc_full_info(0,info); // 0 means current process
	}
	

