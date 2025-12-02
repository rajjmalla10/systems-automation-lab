#include <stdio.h>
int main(){
    char buffer[100];
    FILE *file;
    file = fopen("data.txt","w");

    while (fgets(buffer,sizeof(buffer),file)!=NULL)
    {
        prinf("Read: %s",buffer);

    }
    return 0;
}