#!/bin/bash

program="./nbody"  # Replace with the actual name of your program
particle_file="particles"
problem_size=100  # Adjust the problem size based on your needs
steps=10000


for threads in 1 2 4 8 16; do
    echo "Running with $threads threads"
    ./genparticles $problem_size $particle_file 
    export OMP_NUM_THREADS=$threads
    time $program $particle_file $particle_file $steps
    # Collect and analyze results as needed
done