#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
	int process_rank, size_of_cluster, message_items;
	MPI_Init(&argc,&argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	if(process_rank == 0){
		message_items = 42;
		MPI_Send(&message_items,1,MPI_INT,1,1,MPI_COMM_WORLD);
		printf("Sending message contaning: %d\n",message_items);
		}
	else if(process_rank == 1){
		MPI_Recv(&message_items,1,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf(" Process %d Recieved message contaning: %d from process 0 \n",process_rank,message_items);
		}	
	MPI_Finalize();
	return 0;
	}
