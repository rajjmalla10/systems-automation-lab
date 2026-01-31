#include "proc_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define READ_SIZE 256
#define CARRY_SIZE 1024


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
	int found_pid = 0,  found_name = 0 ;	
	
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
					
					}
				else if((strncmp(line,"Name:",5) == 0)){	
					
					char* p = line + 5;
					while( *p ==' ' || *p == '\t') p++;
					strncpy((*info).name,p,sizeof((*info).name)-1);
					(*info).name[sizeof((*info).name)-1] = '\0';
					found_name = 1;
					
				}
				
				else if(strncmp(line,"State:",6) == 0){
					char* p = line + 6;
					if( *p == ' ' || *p == '\t') p++;
					info->state = *p;
					
					
					}
					
				else if(strncmp(line,"Threads:",8) == 0){
					info->num_threads = atoi(line + 8);
					
					}	
					
					
					
						
				
				carry_buffer[length_tracker] = current;
				
				offset_tracker = 1 + length_tracker;
				
				//early exit if we found all goal we need and clear memory as well
				
				
				if(found_name && found_pid ){
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
	
	char* field_parsing = end_point + 2;
	
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
		info->cutime = cutime;
		info->cstime = cstime;
		info->starttime = starttime;
		return 0;
    }	
	
	return -1;
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
		fprintf(stderr,"Warning: could not read stat file, continuing with basic info\n");
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
	
int read_proc_self_full_info(struct proc_info *info){
	return read_proc_full_info(0,info); // 0 means current process
	}
	

