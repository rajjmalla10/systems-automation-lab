/*
 * process1.c
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

// how process are born: fork() and exec()
// VERY IMPORTANT CONCEPT

//fork() : fork creates copy if current process, 2. both continues running for next line, child gets return value 0, parents gets child's PID

#include <stdio.h>
#include <unistd.h> 
#include <stdio.h>
#include <sys/wait.h>
int main()
{	
	int id = fork();
	
	int n; 
	if(id == 0){
		printf("%d\n",id);
		n = 1;
	}else{
		printf("%d\n ",id);
		n=6;
		}
	if(id!=0){
		wait(NULL);
		}	
		
	int i;
	for(i = n; i < n + 5; i++){
		printf("%d: ",i);
		fflush(stdout);
		}
	printf("\n");	
	return 0;	
		
		
		
		
	
}

