#include <stdio.h>
#include <stdlib.h>

void allocateMemory(int **ptr, int size){
    *ptr = malloc(size * (sizeof(int)));
}

int main(){
    int *ptr;
    allocateMemory(&ptr,10);
    free(ptr);
    return 0;
}