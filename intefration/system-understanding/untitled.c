/*
 * untitled.c
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
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	printf("Testing Process Reading...\n");
	
	//open proc 
	
	int fd = open("/proc/1/stat",O_RDONLY);
	if(fd < 0){
		printf("cannot read /proc/1/stat\n");
		return 1;
		}
	char buffer[256];
	ssize_t bytes_weight;
	if((bytes_weight=read(fd,buffer,sizeof(buffer)))>0){
		write(1,buffer,bytes_weight);
		}else{
			perror("bytes read error");
			return 1;
			}
	close(fd);		
	printf("sucess! just read bytes of system info.\n");
	return 0;
}

