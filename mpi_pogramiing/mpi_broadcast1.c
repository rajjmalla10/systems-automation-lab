#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_bcast(void* data,int count,MPI_Datatype datatype,int rank,MPI_Comm communicator){
	int Bcast_rank, Bcast_size; 
	MPI_Comm_rank(MPI_COMM_WORLD,&Bcast_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&Bcast_size);
	if(Bcast_rank == rank){
		int  i; 
		for(i = 0; i < Bcast_size;i++){
			if(i!=Bcast_rank){
				MPI_Send(data,count,datatype,i,0,communicator);
				}
			}
		}
	else{
		MPI_Recv(data,count,datatype,rank,0,communicator,MPI_STATUS_IGNORE);
		}
	}

int main(int argc , char** argv){
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int data;
	if(rank == 0){
		data = 100;
		my_bcast(&data,1,MPI_INT,0,MPI_COMM_WORLD);
		printf("Process %d send data %d to 3 other processes\n",rank,data);
	}else{
		my_bcast(&data,1,MPI_INT,0,MPI_COMM_WORLD);
		printf("Process %d received data %d from root process\n", rank, data);
		}
	MPI_Finalize();
	return 0;
	}
