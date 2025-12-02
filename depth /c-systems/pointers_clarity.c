#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("Pointer clearance doubts \n");

    int x = 42;

    int *ptr = &x; //stores address of x 
    int **ptr_to_ptr = &ptr; //pointer to a pointer 

    printf("x: %d (the value) \n", x);
    printf("&x: %p (address of x) \n", (void*)&x);
    printf("*ptr %p");
    return 0;


}