/*
 * exc1.c
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
	
{
	(void) argc;
    (void) argv;
	char *filename = "output.txt";
	
	//open a file
	
	int file_fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(file_fd == -1){
		const char *msg = "Eroor opening file\n";
		write(2,msg,strlen(msg)); //stderr
		exit(1);
		}
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;
	while((bytes_read = read(0,buffer,BUFFER_SIZE))>0)
	{
		write(1,buffer,bytes_read);
		write(file_fd,buffer,bytes_read);
		}
	close(file_fd);
		
	
	return 0;
}

