#!/bin/bash

# Function to generate the SLURM script
generate_slurm_script() {
    local X_DIM=$1
    local Y_DIM=$2
    local nodes=$3
    local ompthreads=$4
    local script_name=$5

    output_file="measurements/${X_DIM}x${Y_DIM}_nodes=${nodes}_ompthreads=${ompthreads}.out"

    cat << EOF > $script_name
#!/bin/bash

#SBATCH -J hybrid

#SBATCH -o $output_file
#SBATCH -e $output_file

#SBATCH --time=00:01:00
#SBATCH --account=h039v
#SBATCH --partition=test

#SBATCH --nodes=$nodes
#SBATCH --ntasks=$nodes
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=$ompthreads

module load slurm_setup

# Set number of nodes and tasks correctly!
# Split for node distribtion.
X_DIM=$X_DIM
Y_DIM=$Y_DIM

export OMP_NUM_THREADS=$ompthreads
export KMP_AFFINITY=granularity=fine,scatter

for run in {1..5}; do
    mpiexec -n \$SLURM_NTASKS ./heat test.dat heat.ppm \$X_DIM \$Y_DIM
done
EOF
}

wait_for_jobs_to_finish() {
    while squeue -u $USER | grep -q " R \| PD "; do
        echo "Waiting for job queue to be empty..."
        sleep 2
    done
}

# Split for nodes
dim_pairs=(
    "1 2"
    "2 1"
    "1 3"
    "3 1"
)

for pair in "${dim_pairs[@]}"; do
    X_DIM=$(echo $pair | cut -d' ' -f1)
    Y_DIM=$(echo $pair | cut -d' ' -f2)
    nodes=$((X_DIM * Y_DIM))
    ompthreads=48
    script_name="${X_DIM}x${Y_DIM}_nodes=${nodes}_ompthreads=${ompthreads}.scp"

    generate_slurm_script $X_DIM $Y_DIM $nodes $ompthreads $script_name

    sbatch $script_name

    rm $script_name

    # Job queue has pending limit, must wait.
    wait_for_jobs_to_finish
done

