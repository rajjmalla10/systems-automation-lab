#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;

}

int main(){
    int a = 12, b = 20;
    swap(&a , &b);
    printf("after swap of a = %d and b = %d:",a,b);
    return 0;
    
}