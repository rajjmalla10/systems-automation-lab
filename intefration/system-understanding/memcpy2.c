/*
 * memcpy2.c
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
#include <stdlib.h> 
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define read_size 16 
#define carry_size 4096

int main()
{	
	int fd = open("/proc/meminfo",O_RDONLY);
	if(fd == -1){
		perror("open");
		return 1;
		}
	char read_buf[read_size];
	char carry_buf[carry_size];
	size_t carry_len = 0;
	ssize_t bytes_reads;
	while((bytes_reads = read(fd,read_buf,read_size)) > 0) {
		if(carry_len + bytes_reads >= carry_size){
			break;
			}
			
		memcpy(carry_buf + carry_len, read_buf, bytes_reads);
		carry_len += bytes_reads; 
		
		// extract complete line from carry buffer. 
		
		ssize_t start = 0; // start is the index where the current line begins. it tracts the bytes
		
		for(size_t i = 0 ; i < carry_len ; i++){
			if(carry_buf[i] == '\n'){
				size_t line_len = i  - start + 1; 
				write(1,carry_buf + start, line_len);
				start = i + 1;
				}
			} 
			
			// move unparse lines to front of the memory address of carry buffer. 
			
			if(start > 0){
				memmove(carry_buf, carry_buf+start, carry_len - start);
				carry_len -= start;
				}
		}
	close(fd);	
	return 0;
}

