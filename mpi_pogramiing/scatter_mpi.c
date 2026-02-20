#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
	int rank, size; 
	int distro_Array[4] = {4,14,22,129};
	int scattered_Data;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int len;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(processor_name,&len);
	MPI_Scatter(&distro_Array,1,MPI_INT,&scattered_Data,1,MPI_INT,0,MPI_COMM_WORLD);
	printf("Hello from processor %s , Process %d has recieved: %d \n",processor_name,rank,scattered_Data);
	MPI_Finalize();
	return 0;
	}
