#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char** argv){

    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Barrier(MPI_COMM_WORLD);
    int root = 0;
    int* array = NULL;
    int chunk = 3;
    int size_ofarray = 12;
    int* recv_buffer = NULL;
    if(rank == root){
         array =(int*)malloc(sizeof(int)*size_ofarray);
         for(int i = 0; i < size_ofarray;i++){
            array[i] = i; 
         }
    
         recv_buffer = (int*)malloc(sizeof(int) * chunk * size );
    } 
    int* local_recv = (int*)malloc(sizeof(int)*chunk); 
    MPI_Scatter(array,chunk,MPI_INT,local_recv,chunk,MPI_INT,root,MPI_COMM_WORLD);
    for(int i =0; i<chunk;i++){
        local_recv[i]*=2;
    }
    
    MPI_Gather(local_recv,chunk,MPI_INT,recv_buffer,chunk,MPI_INT,root,MPI_COMM_WORLD);
    if(rank == root){
        for(int i = 0; i < size_ofarray; i++){
            printf("Index %d: %d\n",i,recv_buffer[i]);
        }
        free(array);
    }
    free(local_recv);
    MPI_Finalize();
    return 0;
}
