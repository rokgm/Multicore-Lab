#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int RANK = -1;
int SIZE = -1;

/*
 * Get ARRAY_SIZE env variable, if defined, otherwise returns 0.
*/
size_t get_array_size() {
    size_t array_size = 0;
    char *envValue = getenv("ARRAY_SIZE");
    if (envValue != NULL) {
        array_size = atoi(envValue);
		if (array_size == 0)
			printf("ARRAY_SIZE is %d\n", array_size);
    } else {
		if (array_size == 0)
        	printf("ARRAY_SIZE environment variable is not set, default is %d.\n", array_size);
	}
    return array_size;
}

/*
 * Initializes and return sum or array.
 */
int set_local_array(int* local_array, int local_size, int start_value) {
    int local_sum = 0; 
    for (int i = 0; i < local_size; i++) {
        local_array[i] = start_value + i;
        local_sum += start_value + i;
    }
    return local_sum;
}

/*
 * Example for 8 processes:
 *  0        1        2        3        4        5        6        7
 *  \       /         \       /         \       /         \       /
 *  0:(0 + 1)          2:(2 + 3)         4:(4 + 5)         6:(6 + 7)
 *      \                /                  \                /
 *       0:(0 + 1 + 2 + 3)                   4:(4 + 5 + 6 + 7)
 *               \                                   /
 *                 0:(0 + 1 + 2 + 3 + 4 + 5 + 6 + 7)
 *  Slash / represent: send to process to the left in the tree, which receives and sums up.
 */
void reduce_and_print_sum(int local_sum) {
    for (int step = 1; step < SIZE; step *= 2) {
        // Processes on the left receive from the right (From those that didn't send
        // in the previous step already; those where stoped with break after send.)
        if (RANK % (2 * step) == 0) {
            int received_sum;
            int source = RANK + step;
            MPI_Recv(&received_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += received_sum;
        } else {
            int target = RANK - step;
            MPI_Send(&local_sum, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
            // Process that sends, is done.
            break;
        }
    }

    if (RANK == 0) {
        printf("Rank %d: Global sum = %d\n", RANK, local_sum);
    }
}

int main(int argc, char *argv[]) {

    size_t array_size = get_array_size();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &SIZE);

    int local_array_size = array_size / SIZE + (RANK < array_size % SIZE);
    int start_value = RANK * (array_size / SIZE) + ((RANK < array_size % SIZE) ? RANK : array_size % SIZE);
    int* local_array = (int *)malloc(local_array_size * sizeof(int));    
    int local_sum = set_local_array(local_array, local_array_size, start_value);

#if 0
    printf("Rank %d: local_array: ", RANK);
    for (int j = 0; j < local_array_size; j++) {
        printf(" %-2d ", local_array[j]);
    }
    printf("\n");
#endif

    reduce_and_print_sum(local_sum);

    free(local_array);
    MPI_Finalize();
    return 0;
}
