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

void exchange_boundaries(double *u, algoparam_t* param, local_process_info* local_process_info,
     MPI_Request requests[8]) {
    int local_size_x = param->local_size_x;
    int local_size_y = param->local_size_y;
    int local_allocated_x = param->local_allocated_x;
    int local_allocated_y = param->local_allocated_y;

    int req_count = 0;

    // The border ones do not send/recv.
    // MPI_PROC_NULL takes care of dummy send/recv on border (returns from func immediately).
    // Message tags are important, correct ones must be paired.
    
    // Send from left to right.
    // If rank != MPI_PROC_NULL, still used so we don't unnecessarily copy buffers, won't be sent.
    if (local_process_info->rank_right != MPI_PROC_NULL) {
        int column = param->local_size_x;
        copy_to_send_buff_y(u, param->send_buff_y_left, param, column);
    }
    MPI_Irecv(param->recv_buff_y_left, local_size_y, MPI_DOUBLE, local_process_info->rank_left, 0,
         local_process_info->comm_cart, &requests[req_count++]);
    MPI_Isend(param->send_buff_y_left, local_size_y, MPI_DOUBLE, local_process_info->rank_right, 0,
         local_process_info->comm_cart, &requests[req_count++]);

    // Send right to left.
    if (local_process_info->rank_left != MPI_PROC_NULL) {
        int column = 1;
        copy_to_send_buff_y(u, param->send_buff_y_right, param, column);
    }
    MPI_Irecv(param->recv_buff_y_right, local_size_y, MPI_DOUBLE, local_process_info->rank_right, 1,
         local_process_info->comm_cart, &requests[req_count++]);
    MPI_Isend(param->send_buff_y_right, local_size_y, MPI_DOUBLE, local_process_info->rank_left, 1,
         local_process_info->comm_cart, &requests[req_count++]);


    // Send down to up.
    MPI_Isend(&u[local_allocated_x + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_up, 2,
                 local_process_info->comm_cart, &requests[req_count++]);
    MPI_Irecv(&u[local_allocated_x * (local_allocated_y - 1) + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_down, 2,
         local_process_info->comm_cart, &requests[req_count++]);

    // Send up to down.
    MPI_Isend(&u[local_allocated_x * (local_allocated_y - 2) + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_down, 3, 
         local_process_info->comm_cart, &requests[req_count++]);
    MPI_Irecv(&u[1], local_size_x, MPI_DOUBLE, local_process_info->rank_up, 3, 
         local_process_info->comm_cart, &requests[req_count++]);   
}

// u1 is the local array, including the overlaps.
double relax_jacobi( double **u1, double **utmp1,
         local_process_info* local_process_info, algoparam_t* param )
{
	double* utmp = *utmp1;
	double* u = *u1;
	double unew, diff, sum = 0.0;

    MPI_Request requests[8] = {};
    exchange_boundaries(u, param, local_process_info, requests); 

    // Iterate over the inner part that isn't affected by communication.
	for(int i = 2; i <= param->local_size_y - 1; i++ ) {
        int ii = i * param->local_allocated_x;
        int iim1 = (i - 1) * param->local_allocated_x;
        int iip1 = (i + 1) * param->local_allocated_x;
        #pragma ivdep
        for(int j = 2; j <= param->local_size_x - 1; j++ ){
            unew = 0.25 * (u[ii + (j-1)]+
                            u[ii + (j+1)]+
                            u[iim1 + j]+
                            u[iip1 + j]);
            diff = unew - u[ii + j];
            utmp[ii + j] = unew;
            sum += diff * diff;
        }
	}

    // Wait for the communication to be over, this points are affected by it.
    MPI_Waitall(8, requests, MPI_STATUSES_IGNORE);
    // Copy buffers to u.
    // Don't copy from an empty buffer -> MPI_PROC_NULL.
    if (local_process_info->rank_left != MPI_PROC_NULL) {
        int column = 0;
        copy_from_recv_buff_y(u, param->recv_buff_y_left, param, 0);
    }
    if (local_process_info->rank_right != MPI_PROC_NULL) {
        int column = param->local_size_x + 1;
        copy_from_recv_buff_y(u, param->recv_buff_y_right, param, column);
    }

    // Upper row
    int i = 1;
    int ii = i * param->local_allocated_x;
    int iim1 = (i - 1) * param->local_allocated_x;
    int iip1 = (i + 1) * param->local_allocated_x;
    #pragma ivdep
    for(int j = 1; j <= param->local_size_x; j++ ){
        unew = 0.25 * (u[ii + (j-1)]+
                        u[ii + (j+1)]+
                        u[iim1 + j]+
                        u[iip1 + j]);
        diff = unew - u[ii + j];
        utmp[ii + j] = unew;
        sum += diff * diff;
    }

    // Lower row
    i = param->local_size_y;
    ii = i * param->local_allocated_x;
    iim1 = (i - 1) * param->local_allocated_x;
    iip1 = (i + 1) * param->local_allocated_x;
    #pragma ivdep
    for(int j = 1; j <= param->local_size_x; j++ ){
        unew = 0.25 * (u[ii + (j-1)]+
                        u[ii + (j+1)]+
                        u[iim1 + j]+
                        u[iip1 + j]);
        diff = unew - u[ii + j];
        utmp[ii + j] = unew;
        sum += diff * diff;
    }

    // For columns the corners should be counted again.
    // Left column
    #pragma ivdep
    for(int i = 2; i <= param->local_size_y - 1; i++ ) {
        int ii = i * param->local_allocated_x;
        int iim1 = (i - 1) * param->local_allocated_x;
        int iip1 = (i + 1) * param->local_allocated_x;

        int j = 1;
        unew = 0.25 * (u[ii + (j-1)]+
                        u[ii + (j+1)]+
                        u[iim1 + j]+
                        u[iip1 + j]);
        diff = unew - u[ii + j];
        utmp[ii + j] = unew;
        sum += diff * diff;
	}

    // Right column
    #pragma ivdep
    for(int i = 2; i <= param->local_size_y - 1; i++ ) {
        int ii = i * param->local_allocated_x;
        int iim1 = (i - 1) * param->local_allocated_x;
        int iip1 = (i + 1) * param->local_allocated_x;

        int j = param->local_size_x;
        unew = 0.25 * (u[ii + (j-1)]+
                        u[ii + (j+1)]+
                        u[iim1 + j]+
                        u[iip1 + j]);
        diff = unew - u[ii + j];
        utmp[ii + j] = unew;
        sum += diff * diff;
	}

	*u1 = utmp;
	*utmp1 = u;

    double global_sum;
    MPI_Allreduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, local_process_info->comm_cart);

	return global_sum;
}


