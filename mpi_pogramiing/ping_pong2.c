#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
int main(int argc, char **argv){
	int rank,size;
	const int ping_pong_limit = 10;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int len;
	MPI_Get_processor_name(processor_name,&len);
	int ping_pong_tracker = 0;
	int partner_destination = (rank+1)%2;
	while(ping_pong_tracker<ping_pong_limit){
		ping_pong_tracker++;
		if(rank == ping_pong_tracker % 2){
			
			MPI_Send(&ping_pong_tracker,1,MPI_INT,partner_destination,0,MPI_COMM_WORLD);
			sleep(1);
			printf("Processor with name %s process %d send incremented ping pong count %d to process %d\n",processor_name,rank,ping_pong_tracker,partner_destination);
		}
		else{
			MPI_Recv(&ping_pong_tracker,1,MPI_INT,partner_destination,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("Processor with name %s  process %d recieved incremented ping pong count %d from process %d\n",processor_name,rank,ping_pong_tracker,partner_destination);
			}	
		}
	MPI_Finalize();
	return 0;	

	}
