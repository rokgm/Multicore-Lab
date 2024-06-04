/*
 * relax_jacobi.c
 *
 * Jacobi Relaxation
 *
 */

#include "heat.h"

void copy_to_send_buff_y(double *u, double *sbuf_y, algoparam_t* param, int col_index)
{
    for (int y = 0; y < param->local_size_y; y++) {
        sbuf_y[y] = u[(y + 1) * param->local_allocated_x + col_index];
    }
}

void copy_from_recv_buff_y(double *u, double *rbuf_y, algoparam_t* param, int col_index)
{
    for (int y = 0; y < param->local_size_y; y++) {
        u[(y + 1) * param->local_allocated_x + col_index] = rbuf_y[y];
    }
}

void exchange_boundaries(double *u, algoparam_t* param, local_process_info* local_process_info) {
    // Get the ranks of the neighboring processes
    // The cartesian communicator was initialized as non periodic, so shift over the border
    // returns MPI_PROC_NULL.
    int rank_left, rank_right, rank_up, rank_down;
    MPI_Cart_shift(local_process_info->comm_cart, 0, 1, &rank_left, &rank_right);
    MPI_Cart_shift(local_process_info->comm_cart, 1, 1, &rank_up, &rank_down);

    int local_size_x = param->local_size_x;
    int local_size_y = param->local_size_y;
    int local_allocated_x = param->local_allocated_x;
    int local_allocated_y = param->local_allocated_y;

    // The border ones do not send/recv.
    // MPI_PROC_NULL takes care of dummy send/recv on border (returns from func immediately).
    // Message tags are important, correct ones must be paired.
    
    // If rank != MPI_PROC_NULL, still used so we don't unnecessarily copy buffers.
    // Send from left to right.
    if (rank_right != MPI_PROC_NULL) {
        copy_to_send_buff_y(u, param->send_buff_y, param, param->local_size_x - 1);
        MPI_Sendrecv(param->send_buff_y, local_size_y, MPI_DOUBLE, rank_right, 0, 
                    param->recv_buff_y, local_size_y, MPI_DOUBLE, rank_left, 0, 
                    local_process_info->comm_cart, MPI_STATUS_IGNORE);
        copy_from_recv_buff_y(u, param->recv_buff_y, param, 1);
    }    

    // // Send right to left.
    // if (rank_left != MPI_PROC_NULL) {
    //     copy_to_send_buff_y(u, param->send_buff_y, param, 1);
    //     MPI_Sendrecv(param->send_buff_y, local_size_y, MPI_DOUBLE, rank_left, 1, 
    //                 param->recv_buff_y, local_size_y, MPI_DOUBLE, rank_right, 1, 
    //                 local_process_info->comm_cart, MPI_STATUS_IGNORE);
    //     copy_from_recv_buff_y(u, param->recv_buff_y, param, param->local_size_x - 1);
    // }

    // // Send down to up.
    // MPI_Sendrecv(&u[local_allocated_x + 1], local_size_x, MPI_DOUBLE, rank_up, 2, 
    //              &u[local_allocated_x * (local_allocated_y - 1) + 1], local_size_x, MPI_DOUBLE, rank_down, 2, 
    //              local_process_info->comm_cart, MPI_STATUS_IGNORE);

    // // Send up to down.
    // MPI_Sendrecv(&u[local_allocated_x * (local_allocated_y - 1) + 1], local_size_x, MPI_DOUBLE, rank_down, 3, 
    //              &u[local_allocated_x + 1], local_size_x, MPI_DOUBLE, rank_up, 3, 
    //              local_process_info->comm_cart, MPI_STATUS_IGNORE);
}

// u1 is the local array, including the overlaps.
double relax_jacobi( double **u1, double **utmp1,
         local_process_info* local_process_info, algoparam_t* param )
{
	double* utmp = *utmp1;
	double* u = *u1;
    // TODO
	double unew, diff, sum = 9999999.0;

    // Y direction requires a send/recv buffers.


    // Iterate over the inner parts of the local array.
    // -> iterate from 1 - local_size.
    // Local allocated size is 0 - local_size + 1.
	for(int i = 1; i < param->local_size_y; i++ ) {
        int ii = i * param->local_size_x;
        int iim1 = (i - 1) * param->local_size_x;
        int iip1 = (i + 1) * param->local_size_x;
        #pragma ivdep
        for(int j = 1; j < param->local_size_x; j++ ){
            unew = 0.25 * (u[ii + (j-1)]+
                            u[ii + (j+1)]+
                            u[iim1 + j]+
                            u[iip1 + j]);
            // diff = unew - u[ii + j];
            utmp[ii + j] = unew;
            // sum += diff * diff;

        }
	}

	*u1 = utmp;
	*utmp1 = u;

	return sum;
}


