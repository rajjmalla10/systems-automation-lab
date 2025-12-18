#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node* link;
};

typedef struct Node* inital_node;

void insert(inital_node* head,int data, int position){
    inital_node temp =(struct Node*)malloc(sizeof(struct Node));
    temp->data= data;
    temp->link=NULL;
    if(position==1){
        temp->link=*head;
        *head=temp;
        return;
    }
    if(*head==NULL){
        printf("error");
        free(temp);
        return;
    }
    inital_node current = *head;
    for(int i=0;i<position-2;i++){
        current = current->link;
    }
    temp->link=current->link;
    current->link= temp;
}

inital_node ReverseNode(inital_node* p){
    
}



void FreeMemory(inital_node* head){
    printf("Freeing list");
    if(*head==NULL){
        printf("there are no nodes or error while loading head");
        return;
    }

    inital_node node_to_delete = *head;
    while (*head!=NULL)
    {
        /* code */
        node_to_delete = *head;
        *head = (*head)->link;
        free(node_to_delete);
    }
    (*head)=NULL;

}




int main(){ 
    inital_node head = NULL;
    insert(&head,20,1);
    insert(&head,22,2);
    insert(&head,24,3);
    insert(&head,25,4);
    head=ReverseNode(&head);
    printList(head);
    FreeMemory(&head);
    return 0;
}