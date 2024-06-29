# Parallelized Jacobi Method for Stationary 2D Heat Equation

Three versions of parallelization:
- MPI with blocking communication
- MPI with non-blocking communication
- Hybrid parallelization with MPI + OpenMP

## Requirements
- Intel MPI
- Intel C Compiler
- (For GCC change flags in Makefile.)

## Building the code
- For blocking version (default):
```shell
make all or make all VARIANT=BLOCKING
```
- For non-blocking version:
```shell
make all VARIANT=NON_BLOCKING 
```
- For hybrid version:
```shell
make VARIANT=HYBRID
```

## Running the code
#### For blocking and non-blocking versions:
```shell
mpiexec -n <number of processes> <options> ./heat test.dat heat.ppm <number of processes in x> <number of processes in y>
```
`<number of processes>` = `<number of processes in x>` $\times$ `<number of processes in y>`
Example usage:
```shell
mpiexec -n 6 -genv I_MPI_DEBUG=5 ./heat test.dat heat.ppm 2 3
```
#### For hybrid version:
```shell
export OMP_NUM_THREADS=<number of omp threads>
export <omp options>
mpiexec -n <number of processes> <options> ./heat test.dat heat.ppm <number of processes in x> <number of processses in y>
```
Each process will have **OMP_NUM_THREADS** of threads.
`<number of processes>` = `<number of processes in x>` $\times$ `<number of processes in y>`

Example usage:
```shell
export OMP_NUM_THREADS=48
export KMP_AFFINITY=granularity=fine,scatter

mpiexec -n 4 -genv I_MPI_DEBUG=5 ./heat test.dat heat.ppm 2 2
```

## SLURM on SUPERMUC
Example for submitting on a SLURM scheduler for hybrid version:
```shell
...
#SBATCH --nodes=4
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=48

module load slurm_setup

# Set number of nodes and tasks correctly!
X_DIM=2
Y_DIM=2

export OMP_NUM_THREADS=48
export KMP_AFFINITY=granularity=fine,scatter

mpiexec -n $SLURM_NTASKS \
    -genv I_MPI_DEBUG=5 \
    ./heat test.dat heat.ppm $X_DIM $Y_DIM
```