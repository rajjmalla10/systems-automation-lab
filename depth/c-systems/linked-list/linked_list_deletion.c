#include<stdio.h>
#include <stdlib.h>

struct Node
{
    /* data */
    int data;
    struct Node* Link;
};


typedef struct Node* inital_node;

void insert(inital_node* head,int data,int position){
    inital_node temp=(struct Node*)malloc(sizeof(struct Node));
    temp->data=data;
    temp->Link=NULL;
    if(position==1){
        temp->Link=*head;
        *head=temp;
        return;
    }
    if(*head==NULL){
        printf("Error: Cannot insert at position %d in empty list",position);
        free(temp);
        return;
    }
    inital_node temp2=*head;
    for(int i=0;i<position-2;i++){
        temp2=temp2->Link;
    }
    temp->Link=temp2->Link;
    temp2->Link=temp;
}

void print(inital_node head){
    printf("Listing the node's data: \n");
    while (head!=NULL)
    {
        /* code */
        printf("%d - ",head->data);
        head=head->Link;
    }
    printf("\n");
}

void FreeMemory(inital_node* head){
    inital_node current=*head;
    int count=0;
    while(current!=NULL){
        inital_node next=current->Link;
        free(current);
        current=next;
        count++;
    }
    *head=NULL;
    printf("%d Node's  memory freed",count);
}

void DeleteNode(inital_node* head,int del_pos){
    if(*head==NULL){
        printf("List is empty nothing to delete.\n");
        return;
    }
    int i;
    if(del_pos==1){
        inital_node temp=*head;
        *head=(*head)->Link;
        free(temp);
        return;
    }
    inital_node temp=*head;
    for(int i=0;i<del_pos-2;i++){
        if(temp->Link==NULL){
            printf("Position %d exceeds list lengths\n",del_pos);
            return;
        }
        temp=temp->Link;

    }
    inital_node node_to_delete=temp->Link;
    temp->Link=node_to_delete->Link;
    free(node_to_delete);
    printf("Node at position %d deleted",del_pos);
}

int main(){
    inital_node head=NULL;
    int n,data,position;
    printf("Enter the number of nodes:\n");
    scanf("%d",&n);
    char del;
    for(int i=0;i<n;i++){
        printf("Enter %d node's data: ",i);
        scanf("%d",&data);
        printf("\nEnter the position: \n");
        scanf("%d",&position);
        insert(&head,data,position);
        print(head);
    }
    printf("Do You want to delete particular Node?!(y/n): \n");
    scanf(" %c",&del);
    if(del == 'y'||del=='Y'){
        int del_pos;
        printf("Enter Position to delete: \n");
        scanf("%d",&del_pos);
        DeleteNode(&head,del_pos);
        print(head);
    }
    FreeMemory(&head);
    return 0;
}