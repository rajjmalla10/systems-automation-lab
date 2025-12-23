/*
 * read-multiple-files.c
 * 
 * Copyright 2025 Raj Malla <raj-malla@raj-malla-Modern-14-B11SB>
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
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096
int main(int argc, char *argv[])
{
	//parse command line
	int append_mode = 0;
	
	int opt;
	
	while((opt = getopt(argc,argv,"a"))!=-1){
		if(opt =='a') append_mode = 1;
		
		}
		//check if optind extends the prompt arrays
		if(optind>=argc){
			const char* err = "usage: tee[-a] file\n";
			write(2,err,strlen(err));
			exit(1);
			}
		//open files
		
		int file_count = argc-optind;
		int file_fds[file_count];
		int flags = O_WRONLY | O_CREAT | (append_mode ? O_APPEND : O_TRUNC);
		for(int i = 0;i<file_count;i++){
			file_fds[i] = open(argv[optind+i],flags,0644);
			if(file_fds[i] == -1){
				const char* err = "Error openning file\n";
				write(2,err,strlen(err));
				exit(1);
				}
				
				
			}
			
		// copy stdin to stdout to all files 
		
		char buffer[BUFFER_SIZE];
		ssize_t bytes;
		
		while((bytes = read(0,buffer,BUFFER_SIZE)) > 0){
			write(1,buffer,bytes);
			for(int i = 0; i < file_count; i++){
				write(file_fds[i],buffer,bytes);
				}
			}
			
		for(int i = 0; i < file_count; i++){
			close(file_fds[i]);
			}			
	
	
	
	return 0;
}

