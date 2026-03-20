#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

int* random_array(int num_of_int){
    int* array = (int*)malloc(sizeof(int)* num_of_int);
    for(int i = 0; i < num_of_int;i++){
        array[i] = rand()%10;
    }
    return array;

}
int main(int argc, char** argv){
    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

  
    
    int root = 0;
    int size_of_array = 5;
    int*  recv_number = NULL;
    if(rank == root){
        recv_number = (int*)malloc(sizeof(int)*size_of_array);
    }
    int* local_number = random_array(size_of_array);
    MPI_Reduce(local_number,recv_number,size_of_array,MPI_INT,MPI_SUM,root,MPI_COMM_WORLD);
    if(rank == root){
       for(int i =0; i < size_of_array; i++){
           printf("index %d: %d\n",i,*(recv_number + i));
       } 
       free(recv_number); 
    }
    free(local_number);
    MPI_Finalize();
    return 0;
}
