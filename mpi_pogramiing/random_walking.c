#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stddef.h>
const int max_walker = 3;
const int domain_size = 20;
const int MAX_WALK_SIZE = 20;
typedef struct {
	int current_pos; 
	int steps_left;
	}walker;	
	
typedef struct {
		walker* data;
		size_t current_size; 
		size_t capacity;
		}vector;	
		
void decompose_domain(int rank,int size, int domain_size, int* subdomain_start, int* subdomain_size ){
	
	if(rank > domain_size){
		MPI_Abort(MPI_COMM_WORLD,1);
			
		}
		*subdomain_start = domain_size / size * rank;
		*subdomain_size = domain_size / size;
			
		if(rank == size - 1){
			*subdomain_size += domain_size % size;
		}
	}
	
void intvector(vector *v, size_t default_capacity){
	v->data = malloc(default_capacity * sizeof(walker));
	v->current_size = 0;
	v->capacity = default_capacity;
	}
		
void pushBack(vector *v,walker w){
	if(v->current_size == v->capacity){
		v->capacity *=2;
		v->data = realloc(v->data,v->capacity*sizeof(walker));
		
		if(v->data == NULL){
			printf("OUt of memeory!\n");
			exit(1);
			}
		
		}
	v->data[v->current_size] = w;
	v->current_size++;
	}		
void freeVector(vector *v){
	free(v->data);
	v->data = NULL;
	v->current_size = 0;
	v->capacity = 0;
	}
void initialize_walker(int num_walker_per_process, int subdomain_size, int subdomain_start, int rank, vector* v){
	walker w;
	for(int i = 0; i < num_walker_per_process; i++){
		w.current_pos = subdomain_start;
		w.steps_left = (int)((rand() / (float)RAND_MAX) * MAX_WALK_SIZE);
		pushBack(v,w);
		}
	}

void walk_the_walker(vector* v, int subdomain_size, int subdomain_start, vector* outgoing_right,vector* outgoing_left){
	for(int i = 0; i < v->current_size;i++){
		while(v->data[i].steps_left > 0){
			v->data[i].steps_left--;
			int direction = (rand() % 2 == 0)?1:-1;
			v->data[i].current_pos+=direction;
			
			if(v->data[i].current_pos >= subdomain_start + subdomain_size){
				pushBack(outgoing_right,v->data[i]);
				v->data[i] = v->data[v->current_size-1];
				v->current_size--;
				i--;
				break;
				}
			if(v->data[i].current_pos < subdomain_start){
				pushBack(outgoing_left,v->data[i]);
				v->data[i] = v->data[v->current_size-1];
				v->current_size--;
				i--;
				break;
				}	
			
			
			}
		}
	} 	

void send_walkers(vector* outgoing_right, vector* outgoing_left, int rank,int size,MPI_Datatype MPI_WALKER){

	//sending right who can 0 ,1 ,2 
	if(rank<size-1){
		int send_right = rank + 1;
		MPI_Send(outgoing_right->data,outgoing_right->current_size,MPI_WALKER,send_right,1,MPI_COMM_WORLD);
		printf("\nProcess %d sending %zu outgoing walker to process %d\n",rank,outgoing_right->current_size,send_right);
		outgoing_right->current_size = 0;
		}
		
	//who can send left 1 ,2 ,3	
	if(rank>0){
		int send_left = rank -1;
		MPI_Send(outgoing_left->data,outgoing_left->current_size,MPI_WALKER,send_left,1,MPI_COMM_WORLD);
		printf("\nProcess %d sending %zu outgoing walker to process %d\n",rank,outgoing_left->current_size,send_left);
		outgoing_left->current_size = 0;
		}	
	
	
	
	}	

void recv_walker(int rank,vector* recived_vector,MPI_Datatype MPI_WALKER){
	
	MPI_Status status;
	int incomming_walker_size;
	MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	MPI_Get_count(&status,MPI_WALKER,&incomming_walker_size);
	if(incomming_walker_size > 0){
		while(incomming_walker_size + recived_vector->current_size >= recived_vector->capacity){
		
		recived_vector->capacity *=2;
		recived_vector->data = realloc(recived_vector->data,recived_vector->capacity*sizeof(walker));
		}
		}
	
	MPI_Recv(&recived_vector->data[recived_vector->current_size],incomming_walker_size,MPI_WALKER,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);
	recived_vector->current_size+=incomming_walker_size;	
		
	printf("Process %d recieved %d incoming walkers\n",rank,incomming_walker_size);
	
	}	

int main(int argc , char **argv){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	MPI_Datatype MPI_WALKER;		
	int blocklengths[2] = {1,1};
	MPI_Datatype type[2] = {MPI_INT,MPI_INT};
	MPI_Aint displacement[2];
	displacement[0] = offsetof(walker,current_pos);
	displacement[1] = offsetof(walker,steps_left);
	MPI_Type_create_struct(2,blocklengths,displacement,type,&MPI_WALKER);
	MPI_Type_commit(&MPI_WALKER);
	
	
	int subdomain_start;
	int subdomain_size;
	vector v;
	vector outgoing_right;
	vector outgoing_left;
	
	if(rank>domain_size){
		MPI_Abort(MPI_COMM_WORLD,1);
		}
		
	decompose_domain(rank,size,domain_size, &subdomain_start, &subdomain_size );
	intvector(&v,7);
	intvector(&outgoing_right,7);
	intvector(&outgoing_left,7);
	initialize_walker(max_walker,subdomain_size,subdomain_start,rank,&v);
	
	printf("process %d initalized %d walker in subdomain %d - %d\n",rank,max_walker,subdomain_start, subdomain_start + subdomain_size);
	int max_round = MAX_WALK_SIZE / subdomain_size;
	int round=0;
	while(round < max_round){
		walk_the_walker(&v, subdomain_size, subdomain_start, &outgoing_right, &outgoing_left);
		printf("[Rank %d][Round %d] walkers in v: %zu\n", 
        rank, round, v.current_size);
		if(rank%2 == 0){
			
			//where can even send? both left and right. and for greater than 0 and less than 3 is handled by send function.
			send_walkers(&outgoing_right,&outgoing_left,rank,size,MPI_WALKER);
			//now even number recv from 
			if(rank>0)recv_walker(rank,&v,MPI_WALKER);
			
			if(rank<size-1)recv_walker(rank,&v,MPI_WALKER);
			
			}
		else{
			if(rank>0)recv_walker(rank,&v,MPI_WALKER);
			if(rank<size-1)recv_walker(rank,&v,MPI_WALKER);
					
			send_walkers(&outgoing_right,&outgoing_left,rank,size,MPI_WALKER);
				
			}	
		round++;
	}
	freeVector(&v);	
	freeVector(&outgoing_left);
	freeVector(&outgoing_right);
	MPI_Type_free(&MPI_WALKER);
	MPI_Finalize();
	return 0;	
	}
