#!/bin/bash

# Function to generate the SLURM script
generate_slurm_script() {
    local X_DIM=$1
    local Y_DIM=$2
    local nodes=$3
    local ntasks=$4
    local script_name=$5

    output_file="measurements/${X_DIM}x${Y_DIM}_nodes=${nodes}.out"

    cat << EOF > $script_name
#!/bin/bash

#SBATCH -J blocking

#SBATCH -o $output_file
#SBATCH -e $output_file

#SBATCH --time=00:01:00
#SBATCH --account=h039v
#SBATCH --partition=test

#SBATCH --nodes=$nodes
#SBATCH --ntasks=$ntasks

module load slurm_setup

# Set number of nodes and tasks correctly for x,y dim!
X_DIM=$X_DIM
Y_DIM=$Y_DIM

mpiexec -n \$SLURM_NTASKS ./heat test.dat heat.ppm \$X_DIM \$Y_DIM
EOF
}

wait_for_jobs_to_finish() {
    while squeue -u $USER | grep -q " R \| PD "; do
        echo "Waiting for job queue to be empty..."
        sleep 2
    done
}

# List of (X_DIM, Y_DIM) pairs
# max for 4 nodes is 192 = 12 * 16
dim_pairs=(
    # Go up by socket, node...
    "1 24"
    "1 48"
    "1 96"
    "1 144"
    "1 192" 

    "24 1"
    "48 1"
    "96 1"
    "144 1"
    "192 1"   

    # 1 socket 
    "4 6"
    "6 4"

    # 1 node
    "6 8"
    "8 6"

    # 2 nodes
    "12 8"
    "8 12"

    # 3 nodes
    "12 12"

    # 4 nodes
    "12 16"
    "16 12"
)

for pair in "${dim_pairs[@]}"; do
    X_DIM=$(echo $pair | cut -d' ' -f1)
    Y_DIM=$(echo $pair | cut -d' ' -f2)
    ntasks=$((X_DIM * Y_DIM))
    nodes=$(( (ntasks + 47) / 48 ))
    script_name="${X_DIM}x${Y_DIM}_nodes=${nodes}.scp"

    generate_slurm_script $X_DIM $Y_DIM $nodes $ntasks $script_name

    sbatch $script_name

    rm $script_name

    # Job queue has pending limit, must wait.
    wait_for_jobs_to_finish
done

