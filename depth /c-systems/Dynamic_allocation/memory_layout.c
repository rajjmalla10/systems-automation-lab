#include <stdio.h>
#include <stdlib.h>

void memory_layout_demo(){
    int stack_memory;
    int* heap_memory = (int*)malloc(sizeof(int));
    *heap_memory = 50;

    printf("stack variable address: %p \n", (void*)&stack_memory);
    printf("Heap memory  address %p \n", (void*) heap_memory); // address of memory address heap_memory points to 
    printf("Heap memory actual address %p",(void*)&heap_memory);
    printf("Heap memory stored address value %d\n", *heap_memory);

    free(heap_memory);

}


int main(){
    memory_layout_demo();
    return 0;
}    
