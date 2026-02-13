#include "proc_status.h"
#include "socket_header.h"
#include <stdio.h> 
#include <unistd.h>
#include <dirent.h> //for directory operations
#include <ctype.h> // for isdigit.h()
#include <stdlib.h> // for atoi.h
#include <sys/types.h> // added: for pid_t
#include <errno.h>  
#include <limits.h> 

#ifndef PID_MAX
#define PID_MAX 4194304
#endif
extern proc_node_t *index[32768];
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

void update_system_status(proc_node_t **head){
	
	proc_node_t *curr = *head;
	while(curr!=NULL){
		curr->active = 0;
		curr = curr->next;
		}
	}

//helper function: safe string to pid_conversion 

static pid_t safe_str_to_pid(const char* str){
	char *endpoint;
	long val;
	
	errno = 0; // reset error
	
	val = strtol(str,&endpoint,10);
	
	//check for conversion errors , ERANGE : NUMBER TOO LARAGE/SMALL FOR LONG
	if(errno == ERANGE || val<0 || val > PID_MAX || endpoint == str || *endpoint!='\0'){
		return -1;
		}
	
	return (pid_t)val;	
	
}
	

int main(){
	proc_node_t *head = NULL;
	DIR* dir = opendir("/proc");
	if(!dir){
		perror("Failed to open dir");
		return 1;
		}
	DEBUG_PRINT("directory sucesfully opened now scanning processes\n")	
	
	// READ DIRECTORY ENTREIS 
	struct dirent *entry;
	
	int server_fd = init_socket_server();
	if(server_fd == -1) exit(1);
	
	int client_fd = wait_for_client(server_fd);
	if(client_fd == -1) exit(1);
	//loop through the processes now: 
	char big_buffer[BUFFER_SIZE];
	while(1){
		int current_found = 0;
		int current_read = 0;
		int can_read_cpu = 0;
		int cannot_read_cpu = 0;
		if(head!=NULL){
			update_system_status(&head);
			}
	
		while((entry = readdir(dir))!=NULL){
			// Calling the function is_all_digit to check if it is a digit or now then appending the it,
			if(is_all_digit(entry->d_name)){
				pid_t pid = safe_str_to_pid(entry->d_name);
				if (pid <= 0 || pid >= 32768) continue;
				current_found++;
				struct proc_info info;
				printf("DEBUG: Processing PID %d\n", pid);
				
				 
				if(index[pid]!=NULL ){
					
					if(read_proc_full_info(pid,&index[pid]->info) == 0){
						
						index[pid]->active=1;
					
						}

				}else{
					//create new node.
					
					if(read_proc_full_info(pid,&info) == 0){
						//initialzied cpu tackingfor the very first time
						info.cpu_percentage = 0.0f;
						info.last_cpu_ticks = info.utime + info.stime;
						gettimeofday(&info.last_cpu_checks, NULL);
						
						add_node(&head,info);
						index[pid] = head;
						index[pid]->active = 1; //mark as alive
						
					}
				}	
				
				if(index[pid]!=NULL && index[pid]->active == 1){
					current_read ++;
					
					float cpu = calculate_cpu_percentage(index[pid]);
					
					if(index[pid]->info.utime > 0 || index[pid]->info.stime > 0){
						can_read_cpu ++;
						
					}else{
						cannot_read_cpu ++;
					}
					if(current_read<=5){
							printf("PID %5d | %-15s | State: %c | CPU: %5.1f%% | Mem: %ld KB\n",
                               index[pid]->info.pid, 
                               index[pid]->info.name, 
                               index[pid]->info.state,
                               cpu, 
                               index[pid]->info.vmrss);
					}
				}
						
			}	
		}
		proc_node_t *curr = head;
		while(curr!=NULL){
			proc_node_t *nextnode = curr->next;
			if(curr->active == 0){
				index[curr->info.pid] = NULL;
				to_remove(&head,curr);
				}
			curr = nextnode;	
			}
		
		char* p = big_buffer;
		size_t remaning = sizeof(big_buffer);
		curr = head;
		while(curr != NULL){
			if(curr->active){
				// calculate fresh cpu percentage
				float cpu = calculate_cpu_percentage(curr);
				
				int written = snprintf(p,remaning,"%d,%s,%c,%.1f,%ld;",
										curr->info.pid,
										curr->info.name,
										curr->info.state,
										cpu,
										curr->info.vmrss
										
				
				);
				if(written > 0 && (size_t)written < remaning){
					//slide forward
					p += written;
					remaning -= written;
					}
					
					
				
				}
			curr = curr->next;	
			
		}
		
		
		
		if(remaning > 1){
			*p = '\n';
			p++;
			*p = '\0';
			}
			
		send_data(client_fd,big_buffer);	
			
		
		
		
			
		printf("\n Total PIDs found %d\n",current_found);
		printf("Successfully read: %d\n",current_read);
		printf("Kernel/Protected:    %d\n", current_found - current_read);
		printf("---------------------------\n");
		printf("Processes with cpu acess : %d\n", can_read_cpu);
		printf("Processes without cpu acess : %d\n", cannot_read_cpu);	
		rewinddir(dir);
		sleep(2);	
	}	
	
	
	
	closedir(dir);
	
	
	return 0;
	}


