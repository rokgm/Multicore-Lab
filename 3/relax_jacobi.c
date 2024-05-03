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
	printf("Residual: %f\n", sum);
	return sum;
}

/*
 * One Jacobi iteration step
 */
double relax_jacobi(double **u_ptr, double **utmp_ptr, const unsigned sizex, const unsigned sizey,
	 const unsigned int block_size) {

	double* u = *u_ptr;
	double* utmp = *utmp_ptr;
	double unew, diff, sum = 0.0;

#if 1
	for (unsigned int i = 1; i < sizey - 1; i++) {
		for (unsigned int j = 1; j < sizex - 1; j++) {		
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
#elif 0
	// Blocking no.1, slower then above

	for (unsigned int i = 1; i < sizey - 1; i += block_size) {
        for (unsigned int j = 1; j < sizex - 1; j += block_size) {
            // Process each block
            for (unsigned int ii = i; ii < i + block_size && ii < sizey - 1; ii++) {
                for (unsigned int jj = j; jj < j + block_size && jj < sizex - 1; jj++) {
                    unew = 0.25 * (u[ii * sizex + (jj - 1)] +  // left
						u[ii * sizex + (jj + 1)] +  // right
						u[(ii - 1) * sizex + jj] +  // top
						u[(ii + 1) * sizex + jj]); // bottom
                    utmp[ii * sizex + jj] = unew;

					// residual calculation inserted here (done on old matrix values)
                    diff = unew - u[ii * sizex + jj];
                    sum += diff * diff;
                }
            }
        }
    }
#else 
	// Blocking no.2, goal was to remove double check: ii < i + block_size && ii < sizey - 1
	// Also slower.
	unsigned int i, j;
	for (i = 1; i < sizey - block_size; i += block_size) {
		// Iterate only exact divides, minus rightmost and bottom most.
        for (j = 1; j < sizex - block_size; j += block_size) {
            // Process each block
            for (unsigned int ii = i; ii < i + block_size; ii++) {
                for (unsigned int jj = j; jj < j + block_size; jj++) {
                    unew = 0.25 * (u[ii * sizex + (jj - 1)] +  // left
						u[ii * sizex + (jj + 1)] +  // right
						u[(ii - 1) * sizex + jj] +  // top
						u[(ii + 1) * sizex + jj]); // bottom
                    utmp[ii * sizex + jj] = unew;
					// residual calculation inserted here (done on old matrix values)
                    diff = unew - u[ii * sizex + jj];
                    sum += diff * diff;
                }
            }
        }
		// Iterate remaining block on the right.
		for (unsigned int ii = i; ii < i + block_size; ii++) {
                for (unsigned int jj = j; jj < sizex - 1; jj++) {
                    unew = 0.25 * (u[ii * sizex + (jj - 1)] + u[ii * sizex + (jj + 1)] +
						u[(ii - 1) * sizex + jj] + u[(ii + 1) * sizex + jj]);
                    utmp[ii * sizex + jj] = unew;
                    diff = unew - u[ii * sizex + jj];
                    sum += diff * diff;
                }
            }
    }

	// Iterate remaining bottom blocks without the last.
	for (j = 1; j < sizex - block_size; j += block_size) {
		// i value is from previous loop
		for (unsigned int ii = i; ii < sizey - 1; ii++) {
			for (unsigned int jj = j; jj < j + block_size; jj++) {
				unew = 0.25 * (u[ii * sizex + (jj - 1)] + u[ii * sizex + (jj + 1)] +
					u[(ii - 1) * sizex + jj] + u[(ii + 1) * sizex + jj]);
				utmp[ii * sizex + jj] = unew;
				diff = unew - u[ii * sizex + jj];
				sum += diff * diff;
			}
		}
	}
	// Iterate bottom right block remainder. i and j from previous loops.
	for (unsigned int ii = i; ii < sizey - 1; ii++) {
		for (unsigned int jj = j; jj < sizex - 1; jj++) {
			unew = 0.25 * (u[ii * sizex + (jj - 1)] + u[ii * sizex + (jj + 1)] +
				u[(ii - 1) * sizex + jj] + u[(ii + 1) * sizex + jj]);
			utmp[ii * sizex + jj] = unew;
			diff = unew - u[ii * sizex + jj];
			sum += diff * diff;
		}
	}
#endif

	// Swap the pointers, double pointer is necessary so we switch
	// the address outside the function.
	double* temp = *u_ptr;
	*u_ptr = *utmp_ptr;
	*utmp_ptr = temp;
	
	// printf("Residual: %f\n", sum);
	return sum;
}
