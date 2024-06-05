#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdbool.h>

#include "input.h"
#include "heat.h"
#include "timing.h"

double* time;

void usage(char *s) {
	fprintf(stderr, "Usage: %s <input file> [result file]\n\n", s);
}

bool getArgs(int argc, char* argv[], char *resfilename, FILE* infile, FILE* resfile,
	 algoparam_t* param, int rank, int size) {
	// check arguments
	if (argc < 2) {
		usage(argv[0]);
		return false;
	}

	// check input file
	if (!(infile = fopen(argv[1], "r"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);

		usage(argv[0]);
		return false;
	}

	// check result file
	resfilename = (argc >= 3) ? argv[2] : "heat.ppm";

	if (!(resfile = fopen(resfilename, "w"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for writing.\n\n", resfilename);

		usage(argv[0]);
		return false;
	}

	// check input
	if (!read_input(infile, param)) {
		fprintf(stderr, "\nError: Error parsing input file.\n\n");

		usage(argv[0]);
		return false;
	}

	// Get two-dimensional distribution.
	if (argc == 5) {
		param->x_dist = atoi(argv[3]);
		param->y_dist = atoi(argv[4]);
	} else {
		fprintf(stderr, "Two-dimensional distribution args not passed.\n");
		return false;
	}

	return true;
}

void create_cartesian(local_process_info* local_process_info, algoparam_t* param) {
	int ndims = 2;
	int dim[2] = {param->x_dist, param->y_dist};
	int period[2] = {0, 0};
	int reorder = 1;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dim, period, reorder, &local_process_info->comm_cart);
	MPI_Comm_rank(local_process_info->comm_cart, &local_process_info->cart_rank);
	MPI_Cart_coords(local_process_info->comm_cart, local_process_info->cart_rank, 2, local_process_info->cart_coords);

	MPI_Cart_shift(local_process_info->comm_cart, 0, 1, &local_process_info->rank_left, &local_process_info->rank_right);
        MPI_Cart_shift(local_process_info->comm_cart, 1, 1, &local_process_info->rank_up, &local_process_info->rank_down);



#if 0
	// Print cartesian rank and coordinates.
	MPI_Barrier(MPI_COMM_WORLD);
	printf("world rank=%d, cart rank=%d, cart coord=(%d,%d)\n", local_process_info->world_rank,
		local_process_info->cart_rank, local_process_info->cart_coords[0], local_process_info->cart_coords[1]);	
	MPI_Barrier(MPI_COMM_WORLD);
#endif
}

int main(int argc, char *argv[]) {
	local_process_info local_process_info;

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_process_info.world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &local_process_info.world_size);

	int k, ret;
	FILE *infile, *resfile;
	char *resfilename;
	int np, iter, chkflag;

	
	// algorithmic parameters
	algoparam_t param;

	double residual;

	// set the visualization resolution
	param.visres = 100;

	// Set params
	// Set 2D distribution x_dim, y_dim.
	if (!getArgs(argc, argv, resfilename, infile, resfile, &param,
		 local_process_info.world_rank, local_process_info.world_size)) {
		fprintf(stderr, "Args not passed correctly. Aborting!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	// Virtual topology
	// Reorder set to 1 -> Takes hardware topology into account and reorders ranks.
	// New communicator and cartesian rank is stored in local_process_info.
	// Create cartesian coordinates, see advanced mpi lecture slides for visualization.
	create_cartesian(&local_process_info, &param);

	if (local_process_info.world_rank == 0) {
		print_params(&param);
	}

	time = (double *) calloc(sizeof(double), (int) (param.max_res - param.initial_res + param.res_step_size) / param.res_step_size);

	int exp_number = 0;

	// Iterate over resolutions.
	for (param.act_res = param.initial_res; param.act_res <= param.max_res; param.act_res = param.act_res + param.res_step_size) {
		if (exp_number != 0) finalize(&param);

		// Each process will allocate the local array plus the overlap (data will be send to the overlap).
		// For parts on the border of the domain, the overlap will be the boundary condition.
		// Rank 0 will get upper left part of domain, rank 1 will get one to the rigth...
		if (!initialize(&param, &local_process_info)) {
			fprintf(stderr, "Error in Jacobi initialization.\n\n");
			usage(argv[0]);
		}
#if 0
		// Print local array params.
		MPI_Barrier(MPI_COMM_WORLD);
		printf("cart. rank %d: local x=%d, local y=%d, allocated x=%d, allocated y=%d, x start=%d, y start=%d\n",
			local_process_info.cart_rank, param.local_size_x, param.local_size_y, param.local_allocated_x,
             param.local_allocated_y, param.global_start_x, param.global_start_y);	
		MPI_Barrier(MPI_COMM_WORLD);
#endif

		// Copy u to uhelp.
		for (int y = 0; y < param.local_allocated_y; y++) {
			for (int x = 0; x < param.local_allocated_x; x++) {
				param.uhelp[y * param.local_allocated_x + x] = param.u[y * param.local_allocated_x + x];
			}
		}

#if 0	
		// Print uhelp, rank by rank
        for (int rank = 0; rank < local_process_info.world_size; rank++) {
            MPI_Barrier(MPI_COMM_WORLD);
            if (rank != local_process_info.cart_rank)
                continue;

            printf("cart_rank = %d, u = \n", local_process_info.cart_rank);
            for (int y = 0; y < param.local_allocated_y; y++) {
                for (int x = 0; x < param.local_allocated_x; x++) {
                    printf("%f ", param.uhelp[y * param.local_allocated_x + x]);
                }
                printf("\n");
            }
        }
		MPI_Barrier(MPI_COMM_WORLD);
#endif

		// starting time
		time[exp_number] = wtime();
		residual = 999999999;
		np = param.act_res + 2;

		for (iter = 0; iter < param.maxiter; iter++) {
			residual = relax_jacobi(&(param.u), &(param.uhelp), &local_process_info, &param);
			if (residual<0.00000005)break;
		}

		time[exp_number] = wtime() - time[exp_number];

		if (local_process_info.cart_rank == 0) {
			printf("\n\nResolution: %u\n", param.act_res);
			printf("===================\n");
			printf("Execution time: %f\n", time[exp_number]);
			printf("Residual: %f\n\n", residual);

			printf("megaflops:  %.1lf\n", (double) param.maxiter * (np - 2) * (np - 2) * 7 / time[exp_number] / 1000000);
			printf("  flop instructions (M):  %.3lf\n", (double) param.maxiter * (np - 2) * (np - 2) * 7 / 1000000);
		}

		exp_number++;
	}

	// TODO rank 0, reduce, parallelize coarsen
    // param.uvis  = (double*)calloc( sizeof(double),
	// 			      (param.visres+2) *
	// 			      (param.visres+2) );

	// param.act_res = param.act_res - param.res_step_size;

	// coarsen(param.u, param.act_res + 2, param.act_res + 2, param.uvis, param.visres + 2, param.visres + 2);

	// write_image(resfile, param.uvis, param.visres + 2, param.visres + 2);
	// free(param.uvis);

	free(time);
    finalize(&param);

    MPI_Finalize();
	return 0;
}
