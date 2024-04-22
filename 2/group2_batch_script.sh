#!/bin/bash

#SBATCH -J heat

#SBATCH --ear=off

#SBATCH -o job.out
#SBATCH -e job.out

#SBATCH --time=00:05:00
#SBATCH --account=h039vai
#SBATCH --partition=test

srun ./heat test.dat

