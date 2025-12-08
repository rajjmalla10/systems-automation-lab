// Insert the node at any given position on the list

#include <stdio.h>
#include <stdlib.h>

struct Node
{
    /* data */
    int data;
    struct Node* Next;
};

typedef struct Node* initial_node;

void insert(int data ,int pos, initial_node* head){
    initial_node temp1 = (struct Node*)malloc(sizeof(struct Node));
    temp1 ->data=data;
    temp1->Next=NULL;
    if( pos == 1){
        temp1 -> Next = *head;
        *head = temp1;//head->points,to,temp1(pointer-node).
        return;
    } 
    
    initial_node temp2=*head;
    int i;
    for( i = 0; i < pos-2; i++ ){
        temp2= temp2->Next;
    }
    temp1->Next=temp2->Next;
    temp2->Next=temp1;
}

void print(initial_node head){
    printf("List of Node:\n");
    
    while(head!=NULL){
        printf("%d - ",head->data);
        head=head->Next;
    }
    printf("\n");
}

void FreeMemory(initial_node *head){
    printf("Freeing Memory\n");
    int count=0;
    initial_node current= *head;
    while (current!=NULL){
        initial_node next=current->Next;
        free(current);
        current = next;
        count++;
    }
    *head=NULL;
    printf("%d Node Freed",count);
}

int main(){

    initial_node head = NULL;
    printf("Enter no.of position you want: \n");
    int count,data,pos;
    
    scanf("%d",&count);
    
    for (int i = 0; i < count; i++)
    {
        /* code */
        printf("Enter the '%d' data:\n",i);
        scanf("%d",&data);
        printf("Enter the position No: \n");
        scanf("%d",&pos);
        insert(data,pos,&head);
        print(head);
        
    }
    FreeMemory(&head);
    
    return 0;
}
