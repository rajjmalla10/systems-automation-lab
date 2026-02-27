#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
const int TAG_DATA = 1; 
const int TAG_DONE = 0; 
const int min_val = 0;
const int max_val = 96;

int min(int a, int b){
	return (a < b) ? a : b;
	}

int main(int argc, char **argv){
	int size,rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(size!=4){
		fprintf(stderr,"MUst use 4 process!!\n");
		MPI_Abort(MPI_COMM_WORLD,1);
		}
	if(rank!=0){
		int local_start = (rank - 1) * 50;
		int local_end = (rank * 50)-1;
		int effective_start = local_start;
		int effective_end = min(local_end,max_val -1);
		int width = effective_end - effective_start + 1;
		
		if(width > 0){
			srand(time(NULL)+rank);
			int offset = random() % width;
			int value = effective_start+offset;
			MPI_Send(&value,1,MPI_INT,0,1,MPI_COMM_WORLD);
			int done = 1;
			MPI_Send(&done,1,MPI_INT,0,0,MPI_COMM_WORLD);
			printf("worker process %d send random %d to master process 0\n",rank,value);
		}
		else{
			int done = 1;
			MPI_Send(&done,1,MPI_INT,0,0,MPI_COMM_WORLD);
			printf("worker process %d send random NULL to  master process 0\n",rank);
			}
		}
	
	if(rank == 0){
		
		int done_count = 0;
		while(done_count < size -1){
			int count;
			MPI_Status status;
			MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			
			
			if(status.MPI_TAG == TAG_DATA){
				MPI_Get_count(&status,MPI_INT,&count);
				int *buffer = malloc(count * sizeof(int));
				MPI_Recv(buffer,count,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);
				if(count > 0){
					printf("Received %d from worker %d\n", buffer[0], status.MPI_SOURCE);
					}	
				
				free(buffer);
				
				
			}else if (status.MPI_TAG == TAG_DONE){
				MPI_Get_count(&status,MPI_INT,&count);
				int *buffer = malloc(count * sizeof(int));
				MPI_Recv(buffer,count,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);
				done_count++;
				free(buffer);
				}
			
			}
		}
			
		MPI_Finalize();	
		return 0;
		}	
	
	
		
		
	
	
		
	
