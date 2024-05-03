/*
 * relax_jacobi.c
 *
 * Jacobi Relaxation
 *
 */

#include "heat.h"

#include <stdio.h>


/*
 * Residual (length of error vector)
 * between current solution and next after a Jacobi step
*/

double residual_jacobi(double *u, unsigned sizex, unsigned sizey) {
	unsigned i, j;
	double unew, diff, sum = 0.0;

	for (i = 1; i < sizey - 1; i++) {
		for (j = 1; j < sizex - 1; j++) {
			unew = 0.25 * (u[i * sizex + (j - 1)] +  // left
						u[i * sizex + (j + 1)] +  // right
						u[(i - 1) * sizex + j] +  // top
						u[(i + 1) * sizex + j]); // bottom

			diff = unew - u[i * sizex + j];
			sum += diff * diff;
		}
	}

// Just for debugging
	// printf("Residual: %f\n", sum);
	return sum;
}

/*
 * One Jacobi iteration step
 */
double relax_jacobi(double **u_ptr, double **utmp_ptr, unsigned sizex, unsigned sizey) {

	int i, j;
	double* u = *u_ptr;
	double* utmp = *utmp_ptr;
	double unew, diff, sum = 0.0;

	for (i = 1; i < sizey - 1; i++) {
		for (j = 1; j < sizex - 1; j++) {		
			unew = 0.25 * (u[i * sizex + (j - 1)] +  // left
						u[i * sizex + (j + 1)] +  // right
						u[(i - 1) * sizex + j] +  // top
						u[(i + 1) * sizex + j]); // bottom
			utmp[i * sizex + j] = unew;

			// residual calculation inserted here (done on old matrix values) 
			diff = unew - u[i * sizex + j];
			sum += diff * diff;
		}
	}

	// Swap the pointers, double pointer is necessary so we switch
	// the address outside the function.
	double* temp = *u_ptr;
	*u_ptr = *utmp_ptr;
	*utmp_ptr = temp;
	
	// printf("Residual: %f\n", sum);
	return sum;


// Just for debugging
	// double diff, sum = 0.0;

	// for (unsigned i = 1; i < sizey - 1; i++) {
	// 	for (unsigned j = 1; j < sizex - 1; j++) {
	// 		diff = u[i * sizex + j] - utmp[i * sizex + j];
	// 		sum += diff * diff;
	// 	}
	// }
	// printf("Relax:    %f\n", sum);
}
