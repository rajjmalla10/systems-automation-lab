/*
 * untitled.c
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
#include <unistd.h>
int main()
{
	int fd = open("output.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	if(fd == -1){
		printf("Error opening file\n");
		return 1;
	}
	char *message = "My first file!";
	int numWritten = write(fd,message,14);	
	printf("wrote %d bytes\n",numWritten);
	printf("Message was:\n %s",message);
	close(fd);
	return 0;
}


