#include<stdio.h>>

int recursion(int a[], int size, int index){
    printf("before calling 5");
    recursion()
}


int main(){
    int a[] = {1,2,3,4,5};
    int size = sizeof(a)/sizeof(a[0]);
    printf("printing array element in recursion");
    recursion(a,size,0);
}