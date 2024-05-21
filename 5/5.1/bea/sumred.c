#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
// ARRAY_SIZE=15 mpirun -np 4 ./sum2

int main(int argc, char *argv[]){
    // read size of array
    int array_size = 0;
    char *envValue = getenv("ARRAY_SIZE");
    if (envValue != NULL) {
        array_size = atoi(envValue);
		if (array_size == 0)
			printf("ARRAY_SIZE is %d\n", array_size);
    } else {
		if (array_size == 0)
        	printf("ARRAY_SIZE environment variable is not set, default is %d.\n", array_size);
	}

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int standard_local_size = array_size/size; // size of 
    int local_size = standard_local_size; // Size of each local array
    int *array = (int*)malloc(array_size * sizeof(int));
    int local_sum = 0;
    int received_sum = 0;
    int repetitions = size/2; // how many times to repeat the process of sum+reduction
    

    MPI_Barrier(MPI_COMM_WORLD);

    if (size > (array_size/2)) {

    }

    if (rank == size-1) { // the last process sums all the remaining elements of the array
        local_size += array_size%size;}
    int *local_array = (int*)malloc(array_size * sizeof(int));

    // initialize array and divide labour, potentially multiple elements per rank (not just 2)
    if (rank == 0) {
        //  create array
        printf("array size: %d\n", array_size);
        for (int i = 0; i < array_size; i++) {
            array[i] = i + 1;
        }

        // send to other processes
        for (int i = 1; i < size-1; i++) {
            MPI_Send(array + local_size*i, local_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // send the remaining to the last rank
        MPI_Send(array + standard_local_size*( size-1), local_size + (array_size%size), MPI_INT, size-1, 0, MPI_COMM_WORLD);

        // do own sum
        for (int j=0; j < local_size; j++) {
            local_sum += array[j];
        }
    }
    else {
        MPI_Recv(local_array, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // do own sum
        for (int j=0; j < local_size; j++) {
            local_sum += local_array[j];
        }
    }


    // example of process send/receive (pr 2 receives, then sends to pr 0)
    // 0 1 2 3
    // 0   2
    // 0
    // distance of rank that receives the sum
    int send_distance = 1;
    for (int r = 0; r < repetitions; r++) {

        if (rank%(2*send_distance) == 0) { // the even ones receive
            MPI_Recv(&received_sum, 1, MPI_INT, rank+send_distance, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += received_sum;
        }
        else if (rank%(2*send_distance) == send_distance) { // odd ones send 
            MPI_Send(&local_sum, 1, MPI_INT, rank-send_distance, 0, MPI_COMM_WORLD);
        }
        send_distance = send_distance*2;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) { printf("FINAL SUM: %d\n", local_sum); }

    free(array);
    free(local_array);
    
    MPI_Finalize();
    return 0;

}