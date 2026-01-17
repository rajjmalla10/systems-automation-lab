/*
 * memoery_space1.c
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

// DATA SEGMENTS 
int global_init = 10; //initialzied data 
int global_uninit; //BSS
static int static_int = 20; //initialized data;
static int static_uninit; //uninitalized data;
const int const_global = 30; // often in text(read-only)


void func(int arg){ //function in text
 	int local = 40; // stack frame 
 	static int local_static = 0; //data (data presist between calls!)
	
	//HEAP 
	int* heap_var = malloc(sizeof(int)); //heap 
	*heap_var = 40;
	
	printf("Addressess:\n");
	printf("text (func): %p\n",func);
	printf("data (global): %p\n",&global_init);
	printf("BSS(UNINIT): %p\n",&global_uninit);
	printf("stack (local): %p\n",&local);
	printf("Heap:         %p\n",heap_var);
	
	free(heap_var);
	}


int main()
{
	func(100);
	return 0;
}

