#include<stdio.h>
#include<stdlib.h>

int main(){
    //without type casting
    void* generic_pointer = malloc(sizeof(int));
    int* int_ptr = generic_pointer;
    *int_ptr = 50;

    printf("value stored %d \n",*int_ptr);
    free(generic_pointer);

}