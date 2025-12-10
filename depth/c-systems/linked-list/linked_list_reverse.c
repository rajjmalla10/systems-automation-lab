#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node* Link;
};

typedef struct Node* inital_node;

void insert(inital_node* head,int data, int position){
    inital_node temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->Link = NULL;
    if(position==1){
        temp->Link = *head;
        *head = temp;
        return;
    }
    if(*head==NULL){
        printf("Couldnt or error while loading head pointer: ");
    }
    inital_node current = *head;
    for(int i=0;i<position-2;i++){
        current = current->Link;
    }
    temp->Link = current->Link;
    current->Link = temp;

}

void ReverseList(inital_node* head){
    inital_node current = *head;
    inital_node prev = NULL;
    inital_node next = NULL;
    while(current!=NULL){
        next=current->Link;
        current->Link=prev;
        prev=current;
        current=next;
    }
    *head=prev;
    
}

void Print(inital_node head){
    printf("Printing Nodes");
    while (head!=NULL)
    {
        /* code */
        printf("%d - ",head->data);
        head=head->Link;
    }
    printf("\n");

}

void FreeMemory(inital_node* head){
    printf("Freeing List: ");
    while (*head!=NULL)
    {
        inital_node temp=*head;
        *head=(*head)->Link;
        free(temp);
    }
    *head=NULL;
    
}

int main(){
    inital_node head = NULL;
    int n,data,position;
    char a;
    printf("Enter the number of nodes: ");
    scanf("%d",&n);
    for (int i = 0; i < n; i++)
    {
        /* code */
        printf("Enter %d node\n",i+1);
        scanf("%d",&data);
        if(head==NULL){
            insert(&head,data,1);
        }else{
            printf("\nEnter position of the node: ");
            scanf("%d",&position);
            insert(&head,data,position);
        }
        

    }
    printf("Printing the List:");
    Print(head);
    printf("Would you like to Reverse Your List?('y' or 'Y' ==yes), ('n' or 'N' == no):");
    scanf(" %c",&a);
    do{
        printf("Reverse?(y/n): ");
        scanf(" %c",&a);

    }while(a != 'y' && a!='Y' && a!='n' && a!='N');

    if(a=='y' || a == 'Y'){
        ReverseList(&head);
        
    }
    printf("\nAfter Reverse");
    Print(head);
    FreeMemory(&head);
}