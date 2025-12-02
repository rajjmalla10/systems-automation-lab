#include <stdio.h>

int main(){
    FILE *file; //FILE IS A STRUCTURE,file is a pointer to itZ

    //writing to a file 

    file = fopen("data.txt","w");

    if (file == NULL){
        prinf("Error OPening!\n");
        return 1;
    }

    fprintf(file,"hello file world!\n");
    fclose(file);

    //Reading from a file; 

    file = fopen("data.txt","r");
    if (file == NULL){
        printf("Error loading the file!\n");
        return 1;
    }

    char buffer[100];
    int line_count=0;
    while (fgets(buffer,sizeof(buffer), file!=NULL)){
        line_count++;
        printf(" Line %d: %s",line_count, buffer);

    }
    printf("Total line count: %d",line_count);
    fclose(file);
    return 0;
    {
        /* code */
    }
    
}