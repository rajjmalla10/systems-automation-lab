#include "proc_status.h"
#include <stdio.h> 
#include <dirent.h> //for directory operations
#include <ctype.h> // for isdigit.h()
#include <stdlib.h> // for atoi.h
#include <sys/types.h> // added: for pid_t


int is_all_digit(const char* str){
	if(str == NULL || *str =='\0'){
		return 0;
		}
	while(*str != '\0'){
		if(!isdigit(*str)){
			return 0;
			}
		str++;	
		}
	return 1;		
}		


int main(){
	DIR* dir = opendir("/proc");
	if(!dir){
		perror("Failed to open dir");
		return 1;
		}
	DEBUG_PRINT("directory sucesfully opened now scanning processes\n")	
	
	// READ DIRECTORY ENTREIS 
	struct dirent *entry;
	int found_total_process = 0;
	int successfully_read = 0;
	int can_read_cpu = 0;
	int cannot_read_cpu = 0;
	//loop through the processes now: 
	while((entry = readdir(dir))!=NULL){
		// Calling the function is_all_digit to check if it is a digit or now then appending the it,
		if(is_all_digit(entry->d_name)){
			pid_t pid = atoi(entry->d_name);
			if(pid == 0)continue;
			found_total_process++;
			struct proc_info info;
			if(read_proc_full_info(pid,&info) == 0){
				successfully_read++; 
				if(successfully_read<=3){
					printf("PID %d: %-20s State: %c Memory: %ld KB\n",
                    info.pid, info.name, info.state, info.vmrss);
				}
				if(info.utime > 0 || info.stime > 0){
					can_read_cpu ++;
					
					}else{
						cannot_read_cpu ++;
						}
				DEBUG_PRINT("PID %d: %s (State: %c, Memory: %ld KB)\n",info.pid,
				info.name,info.state,info.vmrss);
			}
			else{
				DEBUG_PRINT("PID %d: [cannot read]\n",pid);
				}
		}else{
			DEBUG_PRINT("follwoing file name was not a digit!!\n")	
		}
	}
	
	closedir(dir);
	printf("\n Total PIDs found %d\n",found_total_process);
	printf("Successfully read: %d\n",successfully_read);
	printf("Failed to read: %d\n", found_total_process - successfully_read);
	printf("Processes with cpu acess : %d\n", can_read_cpu);
	printf("Processes without cpu acess : %d\n", cannot_read_cpu);
	
	return 0;
	}


