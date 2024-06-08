/*
 * misc.c
 *
 * Helper functions for
 * - initialization
 * - finalization,
 * - writing out a picture
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <omp.h>

#include "heat.h"

/*
 * Initialize the iterative solver
 * - allocate memory for matrices
 * - set boundary conditions according to configuration
 */
int initialize( algoparam_t *param, local_process_info* local_process_info)
{
	// Number of inner points, excluding border.
	unsigned int np_core = param->act_res;

	// Doesn't include overlaps/border.
	int core_x_length = np_core / param->x_dist;
	int core_y_length = np_core / param->y_dist;
	// If x or y coordinate is smaller then the remainder of x or y split we add 1.
	core_x_length += local_process_info->cart_coords[0] < (np_core % param->x_dist);
	core_y_length += local_process_info->cart_coords[1] < (np_core % param->y_dist);

	param->local_size_x = core_x_length;
	param->local_size_y = core_y_length;
	// Add overlaps/border + 2.
	param->local_allocated_x = core_x_length + 2;
	param->local_allocated_y = core_y_length + 2;

	// Calculate start position in domain for the core/inner part.
	// Add 1 because start of inner part of bordering arrays is at position domain(1,1).
	param->global_start_x = 1 
		 + local_process_info->cart_coords[0] * (np_core / param->x_dist)
		 // If the x coord is smaller the remainder we add x coord to account for all increments in arrays to the left.
		 + ((local_process_info->cart_coords[0] < np_core % param->x_dist)
		 	? local_process_info->cart_coords[0] : np_core % param->x_dist);

	param->global_start_y = 1 
		 + local_process_info->cart_coords[1] * (np_core / param->y_dist)
		 // If the y coord is smaller the remainder we add y coord to account for all increments in arrays to the left.
		 + ((local_process_info->cart_coords[1] < np_core % param->y_dist)
		 	? local_process_info->cart_coords[1] : np_core % param->y_dist);

    //
    // allocate memory
    //

    (param->u) = (double*)malloc(param->local_allocated_x * param->local_allocated_y * sizeof(double));
    (param->uhelp) = (double*)malloc(param->local_allocated_x * param->local_allocated_y * sizeof(double));

	// First touch allocation
	#pragma omp parallel
	{
		#pragma omp for
		for (int y = 0; y < param->local_allocated_y; y++){
			for (int x = 0; x < param->local_allocated_x; x++){
				param->u[y * param->local_allocated_x + x] = 0;
				param->uhelp[y * param->local_allocated_x + x] = 0;
			}
		}
	}

	(param->send_buff_y_left) = (double*)calloc(param->local_size_y, sizeof(double));
	(param->send_buff_y_right) = (double*)calloc(param->local_size_y, sizeof(double));
	(param->recv_buff_y_left) = (double*)calloc(param->local_size_y, sizeof(double));
	(param->recv_buff_y_right) = (double*)calloc(param->local_size_y, sizeof(double));

    if( !(param->u) || !(param->uhelp) || !(param->send_buff_y_left) || !(param->recv_buff_y_left)
		 || !(param->send_buff_y_right) || !(param->recv_buff_y_right) )
    {
	fprintf(stderr, "Error: Cannot allocate memory\n");
	return 0;
    }

	// includes border
	unsigned int np = param->act_res + 2;

    for( int i=0; i<param->numsrcs; i++ ) {
		/* top row */
		if (local_process_info->cart_coords[1] == 0) {
			for( int x = 0; x < param->local_allocated_x; x++ ) {
				int j = x + param->global_start_x - 1;	// -1 becuase we include border/overlap
				double dist = sqrt( pow((double)j/(double)(np-1) -
						param->heatsrcs[i].posx, 2)+
					pow(param->heatsrcs[i].posy, 2));

				if( dist <= param->heatsrcs[i].range )
				{
				(param->u)[x] +=
					(param->heatsrcs[i].range - dist) /
					param->heatsrcs[i].range *
					param->heatsrcs[i].temp;
				}
			}
		}

		/* bottom row */
		if (local_process_info->cart_coords[1] == param->y_dist - 1) {
			for( int x = 0; x < param->local_allocated_x; x++ ) {
				int j = x + param->global_start_x - 1;	// -1 becuase we include border/overlap
				double dist = sqrt( pow((double)j/(double)(np-1) -
						param->heatsrcs[i].posx, 2)+
					pow(1-param->heatsrcs[i].posy, 2));

				if( dist <= param->heatsrcs[i].range )
				{
				(param->u)[(param->local_allocated_y - 1) * param->local_allocated_x + x] +=
					(param->heatsrcs[i].range-dist) /
					param->heatsrcs[i].range *
					param->heatsrcs[i].temp;
				}
			}
		}

		/* leftmost column; For corners will be set again (row has set it already), but just easier this way. */
		if (local_process_info->cart_coords[0] == 0) {
			for( int y = 0; y < param->local_allocated_y; y++ ) {
				int j = y + param->global_start_y - 1; 	// -1 becuase we include border/overlap
				double dist = sqrt( pow(param->heatsrcs[i].posx, 2)+
					pow((double)j/(double)(np-1) -
						param->heatsrcs[i].posy, 2));

				if( dist <= param->heatsrcs[i].range )
				{
				(param->u)[ y * param->local_allocated_x ]+=
					(param->heatsrcs[i].range-dist) /
					param->heatsrcs[i].range *
					param->heatsrcs[i].temp;
				}
			}
		}

		/* rightmost column */
		if (local_process_info->cart_coords[0] == param->x_dist - 1) {
			for( int y = 0; y < param->local_allocated_y; y++ ) {
				int j = y + param->global_start_y - 1;	// -1 becuase we include border/overlap
				double dist = sqrt( pow(1-param->heatsrcs[i].posx, 2)+
					pow((double)j/(double)(np-1) -
						param->heatsrcs[i].posy, 2));

				if( dist <= param->heatsrcs[i].range )
				{
				(param->u)[ (y + 1) * param->local_allocated_x - 1 ]+=
					(param->heatsrcs[i].range-dist) /
					param->heatsrcs[i].range *
					param->heatsrcs[i].temp;
				}
			}
		}
    }

    return 1;
}

/*
 * free used memory
 */
int finalize( algoparam_t *param )
{
    if( param->u ) {
		free(param->u);
		param->u = 0;
    }

    if( param->uhelp ) {
		free(param->uhelp);
		param->uhelp = 0;
    }

	if( param->recv_buff_y_left ) {
		free(param->recv_buff_y_left);
		param->recv_buff_y_left = 0;
    }

	if( param->send_buff_y_left ) {
		free(param->send_buff_y_left);
		param->send_buff_y_left = 0;
    }

	if( param->recv_buff_y_right ) {
		free(param->recv_buff_y_right);
		param->recv_buff_y_right = 0;
    }

	if( param->send_buff_y_right ) {
		free(param->send_buff_y_right);
		param->send_buff_y_right = 0;
    }

    return 1;
}


/*
 * write the given temperature u matrix to rgb values
 * and write the resulting image to file f
 */
void write_image( FILE * f, double *u,
		  unsigned sizex, unsigned sizey )
{
    // RGB table
    unsigned char r[1024], g[1024], b[1024];
    int i, j, k;

    double min, max;

    j=1023;

    // prepare RGB table
    for( i=0; i<256; i++ )
    {
	r[j]=255; g[j]=i; b[j]=0;
	j--;
    }
    for( i=0; i<256; i++ )
    {
	r[j]=255-i; g[j]=255; b[j]=0;
	j--;
    }
    for( i=0; i<256; i++ )
    {
	r[j]=0; g[j]=255; b[j]=i;
	j--;
    }
    for( i=0; i<256; i++ )
    {
	r[j]=0; g[j]=255-i; b[j]=255;
	j--;
    }

    min=DBL_MAX;
    max=-DBL_MAX;

    // find minimum and maximum
    for( i=0; i<sizey; i++ )
    {
	for( j=0; j<sizex; j++ )
	{
	    if( u[i*sizex+j]>max )
		max=u[i*sizex+j];
	    if( u[i*sizex+j]<min )
		min=u[i*sizex+j];
	}
    }


    fprintf(f, "P3\n");
    fprintf(f, "%u %u\n", sizex, sizey);
    fprintf(f, "%u\n", 255);

    for( i=0; i<sizey; i++ )
    {
	for( j=0; j<sizex; j++ )
	{
	    k=(int)(1024.0*(u[i*sizex+j]-min)/(max-min));
		if (k>=1024) k=1023;
	    fprintf(f, "%d %d %d  ", r[k], g[k], b[k]);
	}
	fprintf(f, "\n");
    }
}

void coarsen(double *u, int oldx, int oldy, double *uvis, int newx, int newy, int rank) {
    int stepx = oldx / newx;
    int stepy = oldy / newy;
	int stopx = newx;
    int stopy = newy;

	if (oldx<newx){
	 stopx=oldx;
	 stepx=1;
    }
    if (oldy<newy){
     stopy=oldy;
     stepy=1;
    }

    for (int i = 0; i < stopy; i++) {
    int ii = stepy * i;
    for (int j = 0; j < stopx; j++) {
        int jj = stepx * j;
        double temp = 0.0;
        for (int k = 0; k < stepy; k++) {
            for (int l = 0; l < stepx; l++) {
                if (ii + k < oldy && jj + l < oldx) {
                    temp += u[(ii + k) * oldx + (jj + l)];
                }
            }
        }
        uvis[i * newx + j] = temp / (stepx * stepy);
    }
    } 
}


void receive_merge_uvis(double *local_uvis, double *global_uvis, int local_newx, int local_newy, int newx, int newy, int world_rank, int world_size, MPI_Comm comm_cart, int x_dist, int y_dist) {
    MPI_Status status;
    int src_cart_coord[2];
    int *src_local_newx = (int *)malloc(world_size * sizeof(int));
    int *src_local_newy = (int *)malloc(world_size * sizeof(int));
    int *src_start_valuex = (int *)malloc(world_size * sizeof(int));
    int *src_start_valuey = (int *)malloc(world_size * sizeof(int));
    double **local_uvis_buffers = (double **)malloc(world_size * sizeof(double *));
    MPI_Request *requests = (MPI_Request *)malloc(world_size * sizeof(MPI_Request));

    // receive data from all ranks and compute sizes and starting points
    for (int src_rank = 1; src_rank < world_size; src_rank++) {
        MPI_Recv(src_cart_coord, 2, MPI_INT, src_rank, 0, comm_cart, MPI_STATUS_IGNORE);
        src_local_newy[src_rank] = newx / x_dist + (src_cart_coord[0] < newx % x_dist);
        src_local_newx[src_rank] = newy / y_dist + (src_cart_coord[1] < newy % y_dist);
        src_start_valuey[src_rank] = src_cart_coord[0] * (newx / x_dist) + ((src_cart_coord[0] <= newx % x_dist) ? src_cart_coord[0] : newx % x_dist);
        src_start_valuex[src_rank] = src_cart_coord[1] * (newy / y_dist) + ((src_cart_coord[1] <= newy % y_dist) ? src_cart_coord[1] : newy % y_dist);
        local_uvis_buffers[src_rank] = (double *)calloc(src_local_newx[src_rank] * src_local_newy[src_rank], sizeof(double));
        MPI_Irecv(local_uvis_buffers[src_rank], src_local_newx[src_rank] * src_local_newy[src_rank], MPI_DOUBLE, src_rank, 0, comm_cart, &requests[src_rank]);
    }

	// copy local_uvis data from rank 0 to global_uvis
	for (int i = 0; i < local_newy; i++) {
		for (int j = 0; j < local_newx; j++) {
			global_uvis[i * newx + j] = local_uvis[i * local_newx + j];
		}
	}

    // wait for all receives
    for (int src_rank = 1; src_rank < world_size; src_rank++) {
        MPI_Wait(&requests[src_rank], &status);
    }

    // copy received data to global_uvis
    for (int src_rank = 1; src_rank < world_size; src_rank++) {
        for (int i = 0; i < src_local_newy[src_rank]; i++) {
            for (int j = 0; j < src_local_newx[src_rank]; j++) {
                global_uvis[(i * newx) + (src_start_valuey[src_rank] * newx) + src_start_valuex[src_rank] + j] = local_uvis_buffers[src_rank][i * src_local_newx[src_rank] + j];
            }
        }
        free(local_uvis_buffers[src_rank]);
    }

    free(src_local_newx);
    free(src_local_newy);
    free(src_start_valuex);
    free(src_start_valuey);
    free(local_uvis_buffers);
    free(requests);
}
