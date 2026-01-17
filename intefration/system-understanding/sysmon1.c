/*
 * sysmon1.c
 * 
 * Copyright 2026 Raj Malla <raj-malla@raj-malla-Modern-14-B11SB>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#define READ_SIZE 4
#define CARRY_SIZE 4096

int my_strcmp(const char* line, const char* key){
	while(*key && *line && *line!=':' ){
		if(*line != *key )return 1;
		line++;
		key++;
		
		}
	return (*key == '\0' && *line == ':')?0:1;	
	}

void get_meminfo(long mem_total, long mem_free){
	if(mem_total <= 0 ||  mem_free < 0){
		return;
		
		}else{
			long mem_used = mem_total - mem_free;
			float mem_free_percentage = (mem_free * 100.0f)/mem_total;
			printf("MemTotal: %ld KB\n",mem_total);
			printf("MemFree: %ld KB\n",mem_free);
			printf("MemUsed: %ld KB\n",mem_used);
			printf("\nMemFreePercentage: %.0f%%",mem_free_percentage);
		}
	}

int main()
{	
	int fd  = open("/proc/meminfo",O_RDONLY);
	if(fd < 0){
		perror("open");
		return 1;
		}
	ssize_t bytes_read;
	char carry_buf[CARRY_SIZE];
	char read_buf[READ_SIZE];	
	size_t carry_len = 0;
	long mem_total = 0;
	long mem_free = 0;
	int have_mem_total = 0;
	int have_mem_free = 0;
	int done =0;
	while((bytes_read=read(fd,read_buf,READ_SIZE))>0){
		
		for(ssize_t i =0; i < bytes_read; i++){
			
			
			if(carry_len<CARRY_SIZE){
				carry_buf[carry_len] = read_buf[i];
				carry_len++;
				printf("carry buffer now (%zu bytes): '%.*s'\n", carry_len, (int)carry_len, carry_buf);
				}else{
					fprintf(stderr,"carry buffer overflow!\n");
					break;
				}
			}
		size_t size = 0; // index of carry bytes 
		size_t start = 0;
		
		while(size<carry_len){
			if(carry_buf[size] == '\n'){
				carry_buf[size] = '\0';
				char *line = carry_buf + start;
				
				if(my_strcmp(line,"MemTotal")==0){
					printf("Keymatched!\n");
					if(sscanf(line,"MemTotal: %ld",&mem_total) == 1){
						have_mem_total = 1;
						}
				}		
				else if(my_strcmp(line,"MemFree")==0){
					printf("keymatched!\n");
					if(sscanf(line,"MemFree: %ld",&mem_free) == 1){
						have_mem_free = 1;
						}
					}		
					
					
				if(have_mem_total > 0 && have_mem_free > 0){
					done = 1;
					break;
					}	
					
			//	ssize_t line_len = size - start ;
			//	write(1,line,line_len);
				start = size + 1;
				}
			size++;
				
			} 
			if(done){
				break;
				}
			//shift memory bytes
			if (start > 0){
				for(size_t i =0 ; i <  (carry_len - start) ; i++){
					carry_buf[i] = carry_buf[start+i];
					 
					
					}
				carry_len -= start;
				}
		}
	if (mem_total > 0 && mem_free > 0) {
		get_meminfo(mem_total, mem_free);
		} else {
			fprintf(stderr, "Failed to read memory info\n");
		}	
		//if there are any7 leftover bytes.
	if(carry_len > 0){
		// left over bytes from the last line 
		carry_buf[carry_len] = '\0';
		write(1,carry_buf,carry_len);	
		}
	close(fd);
	return 0;
}


