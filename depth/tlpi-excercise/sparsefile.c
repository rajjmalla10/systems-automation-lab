/*
 * sparsefile.c
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

#define _GNU_SOURCE // required for SEEK_DATA / SEEK_HOLE

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096


int copy_data(int src_fd, int dest_fd, off_t start, off_t end){
	// MOVE CURSOR for both source and dest 
	char buf[BUFFER_SIZE];
	int bytes_read = end - start;
	
	if(lseek(src_fd,start,SEEK_SET) == -1){
		perror("lseek src");
		return -1;
		}
		
	if(lseek(dest_fd,start, SEEK_SET) == -1){
		perror("lseek dest");
		return -1;
		
		}	
		
	while(bytes_read > 0){
		ssize_t to_read = (bytes_read < BUFFER_SIZE)?bytes_read:BUFFER_SIZE;
		ssize_t r = read(src_fd,buf,to_read);
		if(r == -1){
			perror("read");
			return -1;
			}
			
		if( r == 0 ){
			break;
			}	
			
		ssize_t w = write(dest_fd,buf,r);
		if(w!=r){
			perror("write");
			return -1;
			}	
			
		bytes_read -= r;	
			
		}	
		return 0;
	}




int main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(stderr,"usage: %s source dest\n", argv[0]);
		return 1;
		}
	
	//open src file 
	
	int src_fd = open(argv[1], O_RDONLY);
	if(src_fd == -1){
		perror("open source");
		return 1;
		}	
		
	int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(dest_fd == -1){
		perror("open dest");
		close(dest_fd);
		return 1;
		}
	
	struct stat st; // defining stat struct, with variable name st 
	if(fstat(src_fd,&st) == -1){
		perror("fstat");
		close(src_fd);
		close(dest_fd);
		return 1;
		}	
		
	off_t filesize = st.st_size;
	off_t pos = 0; 
	
	// walk the file chunk by chunk 
	
	while(pos<filesize){
		off_t data = lseek(src_fd, pos, SEEK_DATA);
		if(data == -1){
			if(errno == ENXIO)
				break;
			perror("lseek SEEK_DATA");
			break;	
			}
			
		off_t hole = lseek(src_fd, data, SEEK_HOLE);
		if(hole == -1 ){
			perror("lseek SEEK_HOLE");
			break;
			}	
		if(copy_data(src_fd, dest_fd, data, hole) == -1) break;
		
		pos = hole ;
			
		}
	close(src_fd);
	close(dest_fd);
		
		
	return 0;
}

