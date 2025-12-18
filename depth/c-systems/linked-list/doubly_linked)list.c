/*
 * doubly_linked)list.c
 * 
 * Copyright 2025 Raj Malla <raj-malla@raj-malla-Modern-14-B11SB>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
struct Node{
	int data;
	struct Node* next;
	struct Node* prev;
	};
	
	typedef struct Node* inital_node;


struct Node* getNewNode(int data){
	inital_node newNode = (inital_node)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next=NULL;
	newNode->prev = NULL;
	return newNode;
	}

void insertAtBegining(inital_node* head, int data){
	inital_node newNode = getNewNode(data);
	
	if((*head)!=NULL){
		((struct Node*)(*head))->prev = (struct Node*)newNode;
		}
		*head = newNode;
	}


void insertatposition(inital_node* head, int data, int position){
	inital_node newNode = getNewNode(data);

	if(*head==NULL){
		printf("Error while loading head");
		return;
	}
	if(position == 1 || *head == NULL){
		insertAtBegining(head,data);
		return;
		}
	
	inital_node current = *head;
	for(int i = 1; i< position - 1 && (*head)!=NULL; i++){
		 current  = current->next;	 
	}
	newNode->next = current->next;
	newNode->prev = current;
	current->next = newNode;
	
	}
	
struct Node* ReverseNode(inital_node head, inital_node* newTail){
	if(head==NULL || head->next == NULL){
		*newTail = head;
		return head;
}
	inital_node newNode = ReverseNode(head->next,newTail);
	head->next->next = head;
	head->prev = head->next;
	head->next = NULL;
	*newTail = head;
	return newNode; 
	
	}	
	
void printlist(inital_node head){
	inital_node current = head;
	if(head==NULL){
		printf("Error while loading head or deosnt not exist");
		return;
		}
	
	printf("List: ");
	while(current != NULL){
		printf("%d",current->data);
		current= current->next;
		}	
	printf("\n");	
	}	
void freelist(inital_node* head){
	if(*head==NULL){
		printf("Error while loading node or node doesnt exist!!");
		return;
		}
	inital_node node_to_free = *head;
	inital_node nextnode;
	printf("Freeing list...");
	while(*head!=NULL){
		nextnode=node_to_free->next;
		free(node_to_free);
		node_to_free = nextnode;
		}	
		printf("\n All Node Freed!");
		*head = NULL;
	}
int main()
{
	inital_node head = NULL;
	inital_node tail = NULL;
	insertAtBegining(&head,1);
	insertatposition(&head,2,2);
	insertatposition(&head,3,3);
	if(head!=NULL){
		tail=head;
		while(tail->next!=NULL){
			tail = tail->next;
			}
		}
	head = ReverseNode(head,&tail);
	printlist(head);
	freelist(&head);
	return 0;
}

