// insert at begining of the linked list

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node* link;
};



typedef struct Node* head;

void Insert(head* first,int x){
    head temp =(struct Node*) malloc(sizeof(struct Node));
    if(temp==NULL){
        printf("Memory Error\n");
        return ;
    }
    temp -> data = x;
    temp -> link = *first;

    //3. update first pointer to point towards initial node i.e temp. 
    *first = temp;
    
}

void print(struct Node* first){
    
    printf("List is:\n");
    while(first!=NULL){
        printf("%d",first->data);
        first = first -> link;
    }
    printf("\n");
}

void FreeMemory(head* first){
    printf("\n == Freeing Memory == \n");
    head current =  *first;
    int count = 0;

    while (current!=NULL)
    {
        /* code */
        head next = current->link;
        printf("Freeing Node with data: %d\n",current->data);
        free(current);
        current = next;
        count++;
    }

    *first = NULL;
    printf("Freed %d nodes sucesfully!\n",count);

    
}

int main(){
    head first = NULL;
    int n, i,x; 
    printf("Enter Number of node you want to add!!: \n");
    scanf("%d",&n);
    for(i = 0; i<n; i++ ){
        printf("Enter number for node: %d\n",i);
        scanf("%d",&x);
        Insert(&first,x);
        print(first);
    }
    FreeMemory(&first);
    return 0;

}