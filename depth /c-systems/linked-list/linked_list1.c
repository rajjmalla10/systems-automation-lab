#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node* link;
};

typedef struct Node* initial_node;

int main(){

    initial_node Node = NULL;
    initial_node temp  = (struct Node*) malloc(sizeof(struct Node));
    
    if (temp !=NULL){
        temp->data = 100;
        temp->link = NULL;

        Node = temp;
        free(temp);
        Node = NULL;
    }
    return 0;

}