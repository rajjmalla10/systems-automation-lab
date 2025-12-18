#include<stdio.h>
#include<stdlib.h>

struct Node
{
    /* data */
    int data;
    struct Node* Link;
};

typedef struct Node* initial_node;

void  insert(initial_node* head, int data, int position){
    initial_node temp =(struct Node*)malloc(sizeof(struct Node));
    temp->data=data;
    temp->Link = NULL;
    if(position==1){
        temp->Link=*head;
        *head=temp;
        return;
    }
    

    if(*head==NULL){
        printf("Doesnt exist any node!");
        free(temp);
        return;
    }

    initial_node current = *head;

    for (int i = 0; i < position-2; i++)
    {
        if(current->Link==NULL){
            printf("position %d exceeds list length\n",position);
            free(temp);
            return;
        }
        /* code */
        current = current->Link;
    }
    temp->Link = current->Link;
    current->Link = temp;
}

void FreeMemory(initial_node* head){
    printf("Freeing list");
    if(*head==NULL){
        printf("there are no nodes or error while loading head");
        return;
    }

    initial_node node_to_delete = *head;
    while (*head!=NULL)
    {
        /* code */
        node_to_delete = *head;
        *head = (*head)->Link;
        free(node_to_delete);
    }
    (*head)=NULL;

}

void ReverseRecursion(initial_node head){
    if(head==NULL) return;
    ReverseRecursion((head)->Link);
    printf("%d",(head)->data);
}

int main(){
    initial_node head = NULL;
    int n,data,position;
    printf("enter the list: ");
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        printf("Enter %d node",i+1);
        scanf("%d",&data);
        printf("enter position you want to enter node on: ");
        scanf("%d",&position);
        insert(&head,data,position);
        
    }
    printf("Reverse print:");
    ReverseRecursion(head);
    printf("\n");

    FreeMemory(&head);
    return 0;
}