#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


struct proc_status{
	int pid; 
	int ppid;
	long vmrss; //kb
	};




#define READ_SIZE 4
#define CARRY_SIZE 1024




int main(){
	
	int fd = open("/proc/self/status", O_RDONLY);
	if(fd < 0){
		perror("open file");
		return 1;
	
		}
		
	struct proc_status info = {0};
	
		
	size_t carry_len = 0;	
	char read_buffer[READ_SIZE];
	char carry_buffer[CARRY_SIZE];	
	ssize_t bytes_read; 
	
	char* line;
	while((bytes_read=read(fd,read_buffer,READ_SIZE))>0){
		printf("=== READ LOOP START ===\n");
		printf("Read %zd bytes from file\n",bytes_read);
		printf("carry_len before append: %zu \n",carry_len);
		
		printf("Read data (hex): ");
		for(int i = 0; i < bytes_read; i++){
			if(carry_len<CARRY_SIZE){
				carry_buffer[carry_len] = read_buffer[i];
				carry_len++;
			}else{
				fprintf(stderr,"carry buffer overflow\n");
				break;
				}
			}
			printf("carry_len = %zu, data='%.*s'\n",carry_len,(int)carry_len,carry_buffer);
		
		size_t line_start = 0;
		size_t pos = 0; // it is index of current or more like offset to track the /n i.e new line
		int found_all = 0;
		while(pos<carry_len){
				if(carry_buffer[pos] == '\n'){
					char saved = carry_buffer[pos];
					printf("FOund NewLIne at positon %zu!\n",pos);
					printf("LIne from line_start: %zu to pos: %zu .",line_start,pos);
					carry_buffer[pos] = '\0';
					line = carry_buffer + line_start;
					printf("DEBUG line: '%s'\n", line);
				   
				    
					if((strncmp(line, "Pid:",4))== 0){
						printf("String matched!!\n");
						info.pid=atoi(line+4);
						
						}
					else if(strncmp(line, "PPid:",5)== 0){
						printf("String matched!!\n");
						info.ppid=atoi(line+5);
						printf("DEBUG: parsed PPid:%d\n",info.ppid);
						
					}else if(strncmp(line,"VmRSS:",6)== 0){
						printf("String matched!!\n");
						info.vmrss=atoi(line+6);
						
						}
					carry_buffer[pos] = saved;	
					
					if(info.pid!=0 && info.ppid!=0 && info.vmrss!=0){
						found_all =1;
						break;
						}
						
				  line_start = pos + 1;		
						
				}
				pos++;

			
		}
		
		if(line_start>0){
			
			//shift bytes 
			printf("Shifting: Line_start: %zu, carry_len: %zu\n",line_start,carry_len);
			printf("Before shift first 50 characters: %.*s\n",50,carry_buffer);
			size_t unprocessed_bytes = carry_len - line_start;
			if(unprocessed_bytes>0){
				memmove(carry_buffer,carry_buffer+line_start,unprocessed_bytes);
				}	
			printf("Aftershift, carry_len=%zu\n",carry_len);	
			carry_len = unprocessed_bytes;
			}	
		
		if(found_all){
			break;
			}		
			
						
					
	
	
}
	printf("\npid:%d \n",info.pid);
	printf("\nppid:%d \n",info.ppid);
	printf("\nvmrss:%ld KB\n",info.vmrss);
	if(info.pid == 0){
    printf("WARNING: Pid is 0! Did we not find it?\n");
	}
	if(info.ppid == 0){
    printf("WARNING: PPid is 0! Did we not find it?\n");
	}
	close(fd);
	return 0;
	}

