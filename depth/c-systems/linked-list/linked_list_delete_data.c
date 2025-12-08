#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node* Link;
};

typedef struct  Node* initial_node;

void insert(initial_node *head,int data, int position){
    initial_node temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->Link=NULL;
    if(position==1){
        temp->Link = *head;
        *head=temp;

        return;
    }

    if(*head==NULL){
        printf("Error while loading head");
        free(temp);
        return;
    }

    initial_node temp2 = *head;
    for(int i = 0; i < position - 2 ; i++){
        if(temp2->Link==NULL){
            printf("Position %d exceeds list length\n",position);
            free(temp);
            return;
        }
        temp2 = temp2->Link; 
    }
    temp->Link = temp2->Link;
    temp2->Link = temp;
    
}

void Delete_by_position(initial_node* head, int postion){
    if((*head)==NULL){
        printf("Error While trying to load head\n");
        return;
    }
    
    if(postion==1){
        initial_node temp = *head;
        *head = (*head)->Link;
        free(temp);
        printf("Deleted node at position 1\n");
        return;
    }

    initial_node temp = *head;
    for(int i =0; i<postion-2;i++){
        if(temp->Link==NULL){
            printf("Position %d exceeds the length",postion);
            return;
        }
        temp = temp->Link;
    }
    if(temp->Link==NULL){
        printf("Position %d exceeds list length\n", postion);
        return;
    }
    initial_node node_to_delete = temp->Link;
    temp->Link = node_to_delete->Link;
    free(node_to_delete);
    printf("Node at postion %d deleted:",postion);
}

void Delete_by_value(initial_node* head,int data){
    if((*head)==NULL){
        printf("\nThere are no node present currently..");
        return;
    }
    while(*head != NULL && (*head)->data==data){
        initial_node temp = *head;
        *head = (*head)->Link;
        free(temp);
    }
    initial_node current = *head;
    int delete_count = 0;
    while (current!=NULL && current->Link!=NULL )
    {
        /* code */
        if(current->Link->data==data){
            initial_node delete = current->Link;
            current->Link = delete->Link;
            free(delete);
            delete_count++;
        }
        else{
            current = current->Link;
        }


    }
    if(delete_count > 0){
        printf("Deleted %d occurance(s) of value %d\n",delete_count,data);

    }else{
        printf("Value %d not found\n",data);
    }
    
}

void Delete_by_pointer(initial_node* head,initial_node target){
    if(*head==NULL || target == NULL){
        printf("Invalid head or target value: ");
        return;
    }
    if(*head==target){
        initial_node temp = *head;
        *head = (*head)->Link;
        free(temp);
        printf("Node deleted by pointer reference\n");
        return;
    }
    initial_node current = *head;
    while (current->Link!=NULL && current->Link!=target){
        current = current->Link;
    }
    if(current->Link == NULL){
        printf("Target node not found!!\n");
        return;
    }
    initial_node node_to_delete = current->Link;
    current -> Link = node_to_delete->Link;
    free(node_to_delete);
    printf("Node deleted by pointer reference\n");

}

void Print(initial_node head){
    int n = 0;
    if(head==NULL){
        printf("Node does not exist!! \n");
        return;
    }
    printf("List of Nodes: ");
    while(head!=NULL){
        printf("%d - ",head->data);
        head = head->Link;
        n++;
    }
    printf("\n");
}

void FreeMemory(initial_node* head){
    initial_node delete_node = *head;
    int count =0;
    while (delete_node!=NULL)
    {
        /* code */
        initial_node current = delete_node->Link;
        free(delete_node);
        delete_node = current;
        count++;
    }
    *head=NULL;
    printf("\n%d Node Freed from memory!!",count);

}

int main(){
    initial_node head = NULL;
    int n,i,data,position;
    printf("Enter number of nodes:");
    scanf("%d",&n);
    for(i=0;i<n;i++){
        printf("Enter %d node data: ",i+1);
        scanf("%d",&data);
        printf("Enter Position of %d",i+1);
        scanf("%d",&position);
        insert(&head,data,position);
        
    }
    char choice;
    printf("\n--- Delete by ---\n");
    printf("1. Position\n");
    printf("2. Value\n");
    printf("Enter choice (1 or 2):");
    scanf(" %c",&choice);

    if(choice == '1'){
        printf("Enter Position You want to delete: ");
        scanf("%d",&position);
        Delete_by_position(&head,position);
        

    }else if(choice == '2'){
        printf("Enter value you want to delete: ");
        scanf("%d",&data);
        Delete_by_value(&head,data);
    }
    else if(choice == '3'){
        printf("Delete by pointer, enter value u want to delete: ");
        scanf("%d",&data);
        initial_node found = NULL;
        initial_node current = head;

        while (current!=NULL)
        {
            /* code */
            if(current->data == data){
                found = current;
                break;
            }
            current = current->Link;
        }
        if(found!=NULL){
            printf("Address passed to be deleted!!");
            Delete_by_pointer(&head,found);
        }else{
            printf("%d data not found",data);
        }
        
        
    }



    Print(head);
    FreeMemory(&head);
    return 0;
}
        
