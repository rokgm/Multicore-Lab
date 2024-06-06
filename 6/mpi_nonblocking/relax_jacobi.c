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


void exchange_boundaries(double *u, algoparam_t* param, local_process_info* local_process_info, MPI_Request *req) {
    // Get the ranks of the neighboring processes
    // The cartesian communicator was initialized as non periodic, so shift over the border
    // returns MPI_PROC_NULL.

    // int rank_left, rank_right, rank_up, rank_down;
    // MPI_Cart_shift(local_process_info->comm_cart, 0, 1, &rank_left, &rank_right);
    // MPI_Cart_shift(local_process_info->comm_cart, 1, 1, &rank_up, &rank_down);

    int local_size_x = param->local_size_x;
    int local_size_y = param->local_size_y;
    int local_allocated_x = param->local_allocated_x;
    int local_allocated_y = param->local_allocated_y;

    // The border ones do not send/recv.
    // MPI_PROC_NULL takes care of dummy send/recv on border (returns from func immediately).
    // Message tags are important, correct ones must be paired.

    // Send from left to right.
    // If rank != MPI_PROC_NULL, still used so we don't unnecessarily copy buffers, won't be sent.
    if (local_process_info->rank_right != MPI_PROC_NULL) {
        int column = param->local_size_x;
        copy_to_send_buff_y(u, param->send_buff_y_right, param, column);
    }
    
    MPI_Isend(param->send_buff_y_right, local_size_y, MPI_DOUBLE, local_process_info->rank_right, 0, local_process_info->comm_cart, &(req[0]));
    //printf("World rank %d, Cart rank %d, sending right to cart rank %d\n", local_process_info->world_rank, local_process_info->cart_rank, local_process_info->rank_right);
    MPI_Irecv(param->recv_buff_y_left, local_size_y, MPI_DOUBLE, local_process_info->rank_left, 0, local_process_info->comm_cart, &(req[1]));

    //MPI_Sendrecv(param->send_buff_y, local_size_y, MPI_DOUBLE, rank_right, 0, 
    //            param->recv_buff_y, local_size_y, MPI_DOUBLE, rank_left, 0, 
    //            local_process_info->comm_cart, MPI_STATUS_IGNORE);

    // Don't copy from an empty buffer.
    //if (local_process_info->rank_left != MPI_PROC_NULL) {
    //    int column = 0;
    //    copy_from_recv_buff_y(u, param->recv_buff_y, param, 0);
    //}

    // Send right to left.
    if (local_process_info->rank_left != MPI_PROC_NULL) {
        int column = 1;
        copy_to_send_buff_y(u, param->send_buff_y_left, param, column);
     }

    MPI_Isend(param->send_buff_y_left, local_size_y, MPI_DOUBLE, local_process_info->rank_left, 1, local_process_info->comm_cart, &(req[2]));
    //printf("World rank %d, Cart rank %d, sending left to cart rank %d\n", local_process_info->world_rank, local_process_info->cart_rank, local_process_info->rank_left);

    MPI_Irecv(param->recv_buff_y_right, local_size_y, MPI_DOUBLE, local_process_info->rank_right, 1, local_process_info->comm_cart, &(req[3]));

    //MPI_Sendrecv(param->send_buff_y, local_size_y, MPI_DOUBLE, rank_left, 1, 
    //            param->recv_buff_y, local_size_y, MPI_DOUBLE, rank_right, 1, 
    //            local_process_info->comm_cart, MPI_STATUS_IGNORE);
    // Don't copy from an empty buffer.
    //if (local_process_info->rank_right != MPI_PROC_NULL) {
    //    int column = param->local_size_x + 1;
    //    copy_from_recv_buff_y(u, param->recv_buff_y, param, column);
    //}

    MPI_Isend(&u[local_allocated_x + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_up, 2, local_process_info->comm_cart, &(req[4]));
    //printf("World rank %d, Cart rank %d, sending up to cart rank %d\n", local_process_info->world_rank, local_process_info->cart_rank, local_process_info->rank_up);

    MPI_Irecv(&u[local_allocated_x * (local_allocated_y - 1) + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_down, 2, local_process_info->comm_cart, &(req[5]));
    // Send down to up.
    //MPI_Sendrecv(&u[local_allocated_x + 1], local_size_x, MPI_DOUBLE, rank_up, 2, 
    //             &u[local_allocated_x * (local_allocated_y - 1) + 1], local_size_x, MPI_DOUBLE, rank_down, 2, 
    //             local_process_info->comm_cart, MPI_STATUS_IGNORE);

    MPI_Isend(&u[local_allocated_x * (local_allocated_y - 2) + 1], local_size_x, MPI_DOUBLE, local_process_info->rank_down, 3, local_process_info->comm_cart, &(req[6]));
  //  printf("World rank %d, Cart rank %d, sending down to cart rank %d\n", local_process_info->world_rank, local_process_info->cart_rank, local_process_info->rank_down);

    MPI_Irecv(&u[1], local_size_x, MPI_DOUBLE, local_process_info->rank_up, 3, local_process_info->comm_cart, &(req[7]));
    // Send up to down.
    //MPI_Sendrecv(&u[local_allocated_x * (local_allocated_y - 2) + 1], local_size_x, MPI_DOUBLE, rank_down, 3, 
    //             &u[1], local_size_x, MPI_DOUBLE, rank_up, 3, 
    //             local_process_info->comm_cart, MPI_STATUS_IGNORE);


}



// u1 is the local array, including the overlaps.
double relax_jacobi( double **u1, double **utmp1,
         local_process_info* local_process_info, algoparam_t* param )
{
	double* utmp = *utmp1;
	double* u = *u1;
	double unew, diff, sum = 0.0;

	int start_i = 1;
        int end_i = param->local_size_y;
	int start_j = 1;
        int end_j = param->local_size_x;

	MPI_Request req[8];

	exchange_boundaries(u, param, local_process_info, req);
	MPI_Waitall(8, req, MPI_STATUSES_IGNORE);
//	printf("Done waiting\n");

    	if (local_process_info->rank_left != MPI_PROC_NULL) {
        //printf("%f\n", u[param->local_allocated_x]);
		int column = 0;
        copy_from_recv_buff_y(u, param->recv_buff_y_left, param, 0);
   	//printf("%f\n", u[param->local_allocated_x]);

	}

	if (local_process_info->rank_right != MPI_PROC_NULL) {
        int column = param->local_size_x + 1;
        copy_from_recv_buff_y(u, param->recv_buff_y_right, param, column);
	}


    // Iterate over the inner parts of the local array.
    // -> iterate from 1 - local_size.
    // Local allocated size is 0 - local_size + 1.
    // This is for dealing with the edge case that there is only 1 MPI process, i.e. the process has no neighbours

    if (1) {
    //        if (local_process_info->rank_right == MPI_PROC_NULL && local_process_info->rank_left == MPI_PROC_NULL && local_process_info->rank_up == MPI_PROC_NULL && local_process_info->rank_down == MPI_PROC_NULL) {

	for(int i = 1; i <= param->local_size_y; i++ ) {
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
	    //MPI_Waitall(8, req, MPI_STATUSES_IGNORE);
	}
	if (local_process_info->world_rank == 0) {
	    printf("Sum for no neighbours %f\n", sum);
	} 

	}
	
	} else if (0) {
	printf("Shouldn't be here");
		// More likely scenario, where the MPI process has neighbours it has to exchange data with
	if (local_process_info->rank_up != MPI_PROC_NULL) {
		end_i = param->local_size_y - 1;
	} else {
		end_i = param->local_size_y;
	}

	if (local_process_info->rank_down != MPI_PROC_NULL) {
		start_i = 2;
	} else {
		start_i = 1;
	}

	if (local_process_info->rank_right != MPI_PROC_NULL) {
		end_j = param->local_size_x - 1;
	} else {
		end_j = param->local_size_x;
	}

	if (local_process_info->rank_left != MPI_PROC_NULL) {
		start_j = 2;
	} else {
		start_j = 1;
	}

	//printf("start_i %d, end_i %d, start_j %d, end_j %d", start_i, end_i, start_j, end_j);
        for(int i = start_i; i <= end_i; i++ ) {
        int ii = i * param->local_allocated_x;
        int iim1 = (i - 1) * param->local_allocated_x;
        int iip1 = (i + 1) * param->local_allocated_x;
        #pragma ivdep
        for(int j = start_j; j <= end_j; j++ ){
            unew = 0.25 * (u[ii + (j-1)]+
                            u[ii + (j+1)]+
                            u[iim1 + j]+
                            u[iip1 + j]);
            diff = unew - u[ii + j];
            utmp[ii + j] = unew;
            sum += diff * diff;
        }
	}
 

	printf("Sum after inner iteration %f \n", sum);
	//printf("Wait \n");
	//MPI_Waitall(8, req, MPI_STATUSES_IGNORE);
	
	if (local_process_info->rank_down != MPI_PROC_NULL) {
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
	}

	if (local_process_info->rank_up != MPI_PROC_NULL) {
	int i = param->local_size_y;
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
        }
	

	if (local_process_info->rank_left != MPI_PROC_NULL)  {
	for(int i = 1; i <= param->local_size_y; i++ ) {
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
	}

	if (local_process_info->rank_right != MPI_PROC_NULL) {
	for(int i = 1; i <= param->local_size_y; i++ ) {
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
        }

	}

//	printf("Sum after full iteration %f \n", sum);

	*u1 = utmp;
	*utmp1 = u;

    double global_sum;
    MPI_Allreduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, local_process_info->comm_cart);

	return global_sum;
}


