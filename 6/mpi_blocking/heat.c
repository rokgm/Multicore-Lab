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
		// TODO 
		param->local_size_x = 0;
		param->local_size_y = 0;

		param->local_allocated_x = 0;
		param->local_allocated_y = 0;

		param->global_start_x = 0;
		param->global_start_y = 0;
	} else {
		fprintf(stderr, "Two-dimensional distribution args not passed, aborting.\n");
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	int i, j, k, ret;
	FILE *infile, *resfile;
	char *resfilename;
	int np, iter, chkflag;

	
	// algorithmic parameters
	algoparam_t param;

	double residual;

	// set the visualization resolution
	param.visres = 100;

	// Set params, including the local array size with overlap.
	if (!getArgs(argc, argv, resfilename, infile, resfile, &param, rank, size)) {
		fprintf(stderr, "Args not passed correctly. Aborting!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	if (rank == 0) {
		print_params(&param);
	}

#if DEBUG	
	MPI_Barrier(MPI_COMM_WORLD);
	fprintf(stdout, "Rank %d: local x size = %d, local y size = %d, global x start = %d, global y start = %d\n",
		 rank, param.local_size_x, param.local_size_y, param.global_start_x, param.global_start_y);	
	MPI_Barrier(MPI_COMM_WORLD);
#endif

	time = (double *) calloc(sizeof(double), (int) (param.max_res - param.initial_res + param.res_step_size) / param.res_step_size);

	int exp_number = 0;

	for (param.act_res = param.initial_res; param.act_res <= param.max_res; param.act_res = param.act_res + param.res_step_size) {
		if (param.act_res!=param.initial_res) finalize(&param);

		// Each process will allocate the local array plus the overlap (data will be send to overlap).
		// For parts on the border of the domain, the overlap will be the boundary conditions.
		if (!initialize(&param)) {
			fprintf(stderr, "Error in Jacobi initialization.\n\n");

			usage(argv[0]);
		}

		for (i = 0; i < param.act_res + 2; i++) {
			for (j = 0; j < param.act_res + 2; j++) {
				param.uhelp[i * (param.act_res + 2) + j] = param.u[i * (param.act_res + 2) + j];
			}
		}

		// starting time
		time[exp_number] = wtime();
		residual = 999999999;
		np = param.act_res + 2;

		for (iter = 0; iter < param.maxiter; iter++) {
			residual = relax_jacobi(&(param.u), &(param.uhelp), np, np);
			if (residual<0.00000005)break;
		}

		time[exp_number] = wtime() - time[exp_number];

		if (rank == 0) {
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


    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello, World! from process %d of %d, processor: %s\n", rank, size, processor_name);

    MPI_Finalize();

	return 0;
}
