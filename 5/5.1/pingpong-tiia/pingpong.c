#include "mpi.h"
#include <stdio.h>

#define BUF_SIZE 10

int main(int argc, char *argv[]){
	const int PING_STOP = 10;
	int np, myid;
	MPI_Status status;
	double start_time, end_time, elapsed_time;
	
	MPI_Init(&argc, &argv);
	//if (ierr != MPI_Success){
	//	printf("Error in MPI_Init");
	//}

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (np != 2) {
		fprintf(stderr, "Not 2 processors %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int pingpongs = 0;
	int other_id = (myid + 1) % 2;
       
       	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();

	while (pingpongs < PING_STOP) {
		
		if (myid == pingpongs % 2) {
			pingpongs++;
			char msg[BUF_SIZE] = {pingpongs};
			MPI_Send(&msg, BUF_SIZE, MPI_CHAR, other_id, 10, MPI_COMM_WORLD);
			printf("%d sent msg to %d, ping pong count is %d\n", myid, other_id, pingpongs);
		} else {
			char buf[BUF_SIZE];
			MPI_Recv(&buf, BUF_SIZE, MPI_CHAR, other_id, 10, MPI_COMM_WORLD, &status);
			pingpongs = buf[0];
			printf("%d received msg from %d, ping pong count is %d\n", myid, other_id, pingpongs);	
		}
	}

        MPI_Barrier(MPI_COMM_WORLD);
        end_time = MPI_Wtime();
	
	MPI_Finalize();

        elapsed_time = end_time - start_time;
        printf("%d messages sent in %f seconds\n", pingpongs, elapsed_time);


}
