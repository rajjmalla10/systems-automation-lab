#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char **argv){
	int rank,size; 
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int send_val;
	int counter = size - 1;
	if(size!=4){
		fprintf(stderr,"MUst use 2 process\n");
		MPI_Abort(MPI_COMM_WORLD,1);
		}
	if(rank!=0){
		send_val = rank ;
		MPI_Send(&send_val,1,MPI_INT,0,1,MPI_COMM_WORLD);
		printf("Worker %d sends number %d\n",rank,send_val);
		}
	else if (rank == 0){
		
		for(int i = 1; i < size; i++){
			MPI_Status status;
			int count;
			//blocks untill meassage arrive
			MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			MPI_Get_count(&status,MPI_INT,&count);
			
			//allocate memeory 
			int *buffer = malloc(count * sizeof(int));
			MPI_Recv(buffer,count,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);
			printf("Master recieves %d from woker %d\n",buffer[0],status.MPI_SOURCE);
			free(buffer);
			}
			}
	MPI_Finalize();	
	
	return 0;	
		}	
	
	
