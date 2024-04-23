/*
 * relax_jacobi.c
 *
 * Jacobi Relaxation
 *
 */

#include "heat.h"

#ifdef LIKWID_PERFMON
#include <likwid-marker.h>
#else
#define LIKWID_MARKER_INIT
#define LIKWID_MARKER_THREADINIT
#define LIKWID_MARKER_SWITCH
#define LIKWID_MARKER_REGISTER(regionTag)
#define LIKWID_MARKER_START(regionTag)
#define LIKWID_MARKER_STOP(regionTag)
#define LIKWID_MARKER_CLOSE
#define LIKWID_MARKER_GET(regionTag, nevents, events, time, count)
#endif


/*
 * Residual (length of error vector)
 * between current solution and next after a Jacobi step
*/

double residual_jacobi(double *u, unsigned sizex, unsigned sizey) {
	unsigned i, j;
	double unew, diff, sum = 0.0;

	for (j = 1; j < sizex - 1; j++) {
		for (i = 1; i < sizey - 1; i++) {
			unew = 0.25 * (u[i * sizex + (j - 1)] +  // left
						u[i * sizex + (j + 1)] +  // right
						u[(i - 1) * sizex + j] +  // top
						u[(i + 1) * sizex + j]); // bottom

			diff = unew - u[i * sizex + j];
			sum += diff * diff;
		}
	}
	return sum;
}

/*
 * One Jacobi iteration step
 */
void relax_jacobi(double *u, double *utmp, unsigned sizex, unsigned sizey) {
	LIKWID_MARKER_INIT;
	LIKWID_MARKER_THREADINIT;
	LIKWID_MARKER_START("Compute");
	int i, j;

	for (j = 1; j < sizex - 1; j++) {
		for (i = 1; i < sizey - 1; i++) {
			utmp[i * sizex + j] = 0.25 * (u[i * sizex + (j - 1)] +  // left
						u[i * sizex + (j + 1)] +  // right
						u[(i - 1) * sizex + j] +  // top
						u[(i + 1) * sizex + j]); // bottom
		}
	}

	// copy from utmp to u

	for (j = 1; j < sizex - 1; j++) {
		for (i = 1; i < sizey - 1; i++) {
			u[i * sizex + j] = utmp[i * sizex + j];
		}
	}
	LIKWID_MARKER_STOP("Compute");
	LIKWID_MARKER_CLOSE;
}
