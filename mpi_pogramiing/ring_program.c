#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
	int rank,size;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int len;
	int token;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_MAX_PROCESSOR_NAME[processor_name,&len];
	if(rank!=0){
		MPI_Recv(&token,1,MPI_INT,rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
	else{
		//IF PROCESS = 0 OR RANK = 0
		token = -1;	
		
		}	
	MPI_Send(&token,1,MPI_INT,(rank+1)%size,1,MPI_COMM_WORLD);	
	
	if(rank == 0){
		MPI_Recv(&token,1,MPI_INT,size-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
	MPI_Finalize();
	return 0;
	}
