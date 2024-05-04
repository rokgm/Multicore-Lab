/*
 * heat.h
 *
 * Iterative solver for heat distribution
 */

#include "heat.h"

#include <stdio.h>
#include <stdlib.h>

// #include <likwid-marker.h>

#include "input.h"
#include "timing.h"

void usage(char *s) {
	fprintf(stderr, "Usage: %s <input file> [result file]\n\n", s);
}

// Very important!!!
// utemp (uhelp) must also have boundary set in first iteration.
void apply_boundary(algoparam_t* param, const unsigned int np) {
	for (int i = 0; i < np; i++) {
		param->uhelp[i] = param->u[i];
		param->uhelp[np * (np - 1) + i ] = param->u[np * (np - 1) + i ];
	}
	for (int i = 1; i < np - 1; i++) {
		param->uhelp[i * np] = param->u[i * np];
		param->uhelp[(i+1) * np - 1] = param->u[(i+1) * np - 1];
	}
}

/*
 * Block u and uhelp.
 * Blocks are stored from left to right starting at the top.
 * Blocks are stored after each other as; example for np 13, block size 10
 * 10*10,10*3
 * 3*10, 3*3
 * becomes
 * 10*10,10*3,3*10,3*3
 */
void block_array(algoparam_t* param, const int np, const unsigned int block_size) {
	if (block_size >= np) {
		printf("Array not blocked, block size is bigger then array size.");
		return;
	}

	double* u_blocked = (double*)calloc( sizeof(double),np*np );
	double* uhelp_blocked = (double*)calloc( sizeof(double),np*np );

	unsigned int count = 0;
	for (unsigned int i = 1; i < np - 1; i += block_size) {
        for (unsigned int j = 1; j < np - 1; j += block_size) {
            // Process each block
            for (unsigned int ii = i; ii < i + block_size && ii < np - 1; ii++) {
                for (unsigned int jj = j; jj < j + block_size && jj < np - 1; jj++) {
					count++;
					u_blocked[count] = param->u[ii * np + jj];
					uhelp_blocked[count] = param->uhelp[ii * np + jj];
                }
            }
        }
    }
	// Swap
	double* temp = param->u;
	param->u = u_blocked;
	u_blocked = temp;

	temp = param->uhelp;
	param->uhelp = uhelp_blocked;
	uhelp_blocked = temp;

	// Free memory
	free(u_blocked);
	free(uhelp_blocked);
	u_blocked = 0;
	uhelp_blocked = 0;

	// TODO unblock
}

void run_iterations(algoparam_t* param, const unsigned int np, unsigned int* iter,
	double* residual, unsigned block_size) {
	(*iter) = 0;

	apply_boundary(param, np);

	while (1) {

		switch (param->algorithm) {

		case 0: // JACOBI
			
			*residual = relax_jacobi(&param->u, &param->uhelp, np, np, block_size);
			// residual = residual_jacobi(param->u, np, np);
			break;

		case 1: // GAUSS

			relax_gauss(param->u, np, np);
			*residual = residual_gauss(param->u, param->uhelp, np, np);
			break;
		}

		(*iter)++;

		// solution good enough ?
		if (*residual < 0.000005)
			break;

		// max. iteration reached ? (no limit with maxiter=0)
		if (param->maxiter > 0 && (*iter) >= param->maxiter)
			break;

		// if (iter % 100 == 0)
		// 	fprintf(stderr, "residual %f, %d iterations\n", residual, iter);
	}
}

int main(int argc, char *argv[]) {
	unsigned iter;
	FILE *infile, *resfile;
	char *resfilename;

	// algorithmic parameters
	algoparam_t param;
	int np,i;

	double runtime, flop;
	double residual;
	double time[1000];
	double floprate[1000];
	int resolution[1000];
	int experiment=0;

	// check arguments
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	// check input file
	if (!(infile = fopen(argv[1], "r"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);

		usage(argv[0]);
		return 1;
	}

	// check result file
	resfilename = (argc >= 3) ? argv[2] : "heat.ppm";

	if (!(resfile = fopen(resfilename, "w"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for writing.\n\n", resfilename);

		usage(argv[0]);
		return 1;
	}

	// check input
	if (!read_input(infile, &param)) {
		fprintf(stderr, "\nError: Error parsing input file.\n\n");

		usage(argv[0]);
		return 1;
	}

	print_params(&param);

	// set the visualization resolution
	param.visres = 1024;

	param.u = 0;
	param.uhelp = 0;
	param.uvis = 0;

	param.act_res = param.initial_res;

	char *envValue = getenv("BLOCK_SIZE");
	unsigned block_size = 666;
    if (envValue != NULL) {
        block_size = atoi(envValue);
		printf("BLOCK_SIZE is %d\n", block_size);
    } else
        printf("BLOCK_SIZE environment variable is not set, default is 666.\n");

	// LIKWID_MARKER_INIT;
	// LIKWID_MARKER_THREADINIT;
	// LIKWID_MARKER_START("Compute");

	// loop over different resolutions
	while (1) {

		// free allocated memory of previous experiment
		if (param.u != 0)
			finalize(&param);

		if (!initialize(&param)) {
			fprintf(stderr, "Error in Jacobi initialization.\n\n");

			usage(argv[0]);
		}

		fprintf(stderr, "Resolution: %5u\r", param.act_res);

		// full size (param.act_res are only the inner points)
		np = param.act_res + 2;

		// starting time
		runtime = wtime();
		residual = 999999999;

		run_iterations(&param, np, &iter, &residual, block_size);

		// Flop count after <i> iterations
		flop = iter * 11.0 * param.act_res * param.act_res;
		// stopping time
		runtime = wtime() - runtime;

		fprintf(stderr, "Resolution: %5u, ", param.act_res);
		fprintf(stderr, "Time: %04.3f ", runtime);
		fprintf(stderr, "(%3.3f GFlop => %6.2f MFlop/s, ", flop / 1000000000.0, flop / runtime / 1000000);
		fprintf(stderr, "residual %f, %d iterations)\n", residual, iter);

		// for plot...
		time[experiment]=runtime;
		floprate[experiment]=flop / runtime / 1000000;
		resolution[experiment]=param.act_res;
		experiment++;

		if (param.act_res + param.res_step_size > param.max_res)
			break;
		param.act_res += param.res_step_size;
		
	}

	// LIKWID_MARKER_STOP("Compute");
	// LIKWID_MARKER_CLOSE;

	for (i=0;i<experiment; i++){
		printf("%5d; %5.3f; %5.3f\n", resolution[i], time[i], floprate[i]);

	}

	coarsen(param.u, np, np, param.uvis, param.visres + 2, param.visres + 2);

	write_image(resfile, param.uvis, param.visres + 2, param.visres + 2);

	finalize(&param);

	return 0;
}
