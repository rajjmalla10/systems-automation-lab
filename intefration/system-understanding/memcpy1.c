/*
 * memcpy1.c
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
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>
int main()
{
	char carry[50];
	int carry_len = 0;
	
	char read_buff1[] = "ABCDEFGHIJKLMN"; // 14 bytes 
	char read_buff2[] = "OPQRSTUVWXYZab";
	
	// first copy 
	
	memcpy(carry+carry_len, read_buff1, sizeof(read_buff1));
	carry_len+=14;
	
	
	printf("After first copy \n");
	printf("carry_len = %d\n",carry_len);
	write(1,carry,carry_len);
	printf("\n\n");
	
	
	//second copy 
	
	memcpy(carry + carry_len, read_buff2, 14);
	carry_len += 14; 
	
	printf("After second copy:\n");
	printf("carry_len = %d\n",carry_len);
	write(1,carry,carry_len);
	printf("\n");
	return 0;
}

