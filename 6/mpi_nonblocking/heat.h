/*
 * heat.h
 *
 * Global definitions for the iterative solver
 */

#ifndef JACOBI_H_INCLUDED
#define JACOBI_H_INCLUDED

#include <stdio.h>
#include <mpi.h>

// configuration

typedef struct
{
    float posx;
    float posy;
    float range;
    float temp;
}
heatsrc_t;

typedef struct
{
    unsigned maxiter;       // maximum number of iterations
    unsigned act_res;
    unsigned max_res;       // spatial resolution
    unsigned initial_res;
    unsigned res_step_size;
    unsigned visres;        // visualization resolution

    // Local size of the core local part of the array, excluding the overlapping areas 
    // but including the boundary conditions for outside parts of domain.
    unsigned local_size_x;      
    unsigned local_size_y;

    // local size + 2 to include overlaps and border
    unsigned local_allocated_x;      
    unsigned local_allocated_y; 

    // Global position of the start of the local core array in the whole domain. Excluding the overlaps/borders.
    unsigned global_start_x;     
    unsigned global_start_y;

    // How the domain is split up: 2x2, 2x3, x×y
    int x_dist;     
    int y_dist;

    // Buffer used for vertical communication of size local_size_y.
    double* send_buff_y_left;
    double* recv_buff_y_left;
    double* send_buff_y_right;
    double* recv_buff_y_right;
  
    // Allocated part of the array, overlapping areas are included in here.
    double *u, *uhelp;
    // TODO
    double *uvis;

    unsigned   numsrcs;     // number of heat sources
    heatsrc_t *heatsrcs;
}
algoparam_t;

typedef struct
{
    int world_rank;
    int world_size;

    int cart_rank;
    // (x, y)
    int cart_coords[2];
    MPI_Comm comm_cart;

    int rank_left;
    int rank_right;
    int rank_up;
    int rank_down;
}
local_process_info;


// function declarations

// misc.c
int initialize( algoparam_t *param, local_process_info* local_process_info );
int finalize( algoparam_t *param );
void write_image( FILE * f, double *u,
		  unsigned sizex, unsigned sizey );

void coarsen(double *u, int oldx, int oldy, double *uvis, int newx, int newy, int rank);
void receive_merge_uvis(double *local_uvis, double *global_uvis, int local_newx, int local_newy, int newx, int newy,
 int world_rank, int world_size, MPI_Comm comm_cart, int x_dist, int y_dist);

// Gauss-Seidel: relax_gauss.c
double residual_gauss( double *u, double *utmp,
		       unsigned sizex, unsigned sizey );
void relax_gauss( double *u, 
		  unsigned sizex, unsigned sizey  );

// Jacobi: relax_jacobi.c
double residual_jacobi( double *u,
			unsigned sizex, unsigned sizey );
double relax_jacobi( double **u, double **utmp,
		   local_process_info* local_process_info, algoparam_t* param ); 


#endif // JACOBI_H_INCLUDED
