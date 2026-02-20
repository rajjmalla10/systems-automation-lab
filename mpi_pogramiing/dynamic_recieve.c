#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char **argv){
	int rank,size;
	
	const int MAX_NUMBER = 100;
	int max_number[MAX_NUMBER];
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int message = 0;
	int random_integers;
	if(size!=2){
		fprintf(stderr,"MUst use 2 process\n");
		MPI_Abort(MPI_COMM_WORLD,1);
		}
	while (message < 2){
		if(rank == 0){
			int senders_tag = message;
			message++;
			random_integers = (rand()/(float)RAND_MAX) * MAX_NUMBER;
			MPI_Send(max_number,random_integers,MPI_INT,1,senders_tag,MPI_COMM_WORLD);
			printf("0 sent %d numbers to 1",random_integers);
		}
		else{
			MPI_Status status;
			MPI_Recv(max_number,MAX_NUMBER,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			MPI_Get_count(&status,MPI_INT,&random_integers);
			printf("1 recieved %d numbers from process  %d. Message tag = %d\n",random_integers,status.MPI_SOURCE,status.MPI_TAG);
		}
		}
	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Finalize();
	return 0;	
	}
