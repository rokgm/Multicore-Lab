#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int power(int x, int n) {
    if (n == 0) {
        return 1;
    }
    return x * power(x, n - 1);
}

/**
 * Returns time duration for all send/recv.
*/
double measure_pingpong(const int rank, const int repeats, const int msg_size) {
    int partner_rank = (rank + 1) % 2;

    char *message = (char*)malloc(msg_size);
    memset(message, 'a', msg_size);

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time, end_time;
    int ping_pong_count = 0;

	start_time = MPI_Wtime();
    while (ping_pong_count < repeats) {
        if (rank == ping_pong_count % 2) {
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                    MPI_COMM_WORLD);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
	end_time = MPI_Wtime();

    free(message);
    return end_time - start_time;
}

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        fprintf(stderr, "Pingpong requires exactly 2 processes!\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return EXIT_FAILURE;
    }

	unsigned repeats = 10;
	char *envValue = getenv("NUM_REPEATS");
    if (envValue != NULL) {
        repeats = atoi(envValue);
		if (world_rank == 0)
			printf("NUM_REPEATS is %d\n", repeats);
    } else {
		if (world_rank == 0)
        	printf("NUM_REPEATS environment variable is not set, default is %d.\n", repeats);
	}
    	

	MPI_Barrier(MPI_COMM_WORLD);
	if (world_rank == 0)
    	printf("%-5s  %-15s  %-20s  %-20s\n", "Rank", "Message Size (B)", "Startup Time (ms)", "Bandwidth (MB/s)");

    for (int i = 0; i <= 24; i++) {
        const int msg_size = power(2, i);
        double total_time = measure_pingpong(world_rank, repeats, msg_size);
		// Multiply by 2 as we send twice as much data. Total time also measures time
		// to receive from the other node. Pingpong count is 2 * repeats.
		double startup_time = total_time / (repeats * 2);
		double bandwidth = 2 * (double)(msg_size * repeats) / total_time;
		if (world_rank == 0)
			printf("%-5d  2^%-14d  %-20.6f  %-20.6f\n", world_rank, i, 1000 * startup_time, bandwidth / 1000000);

    }

	// char processor_name[MPI_MAX_PROCESSOR_NAME];
    // int name_len;
    // MPI_Get_processor_name(processor_name, &name_len);

	// MPI_Barrier(MPI_COMM_WORLD);
    // printf("Rank %d finalized on processor: %s\n", world_rank, processor_name);

    MPI_Finalize();
    return 0;
}
