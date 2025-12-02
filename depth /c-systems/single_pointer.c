#include <stdio.h>
#include <stdlib.h>

void modifyValue(int *p){
    *p = 20;
}

int main(){
    int x = 10;
    modifyValue(&x);
    printf("x is now:%d\n",x);
    return 0;
}