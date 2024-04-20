# Multicore Lab SoSe 2024
Group 2: Tiia Tikkala, Beatrice Picco, Rok Grgič Meško

## Assignment 1

### 1.1 Understanding the code
* Show the initial performance data. The form of the graph was introduced in the meeting.
* Explain how the FLOP/s metric is measured. Which floating point operations are taken into account? Only from relax_jacobi.c, 11 flops counted from residual_jacobi and relax_jacobi


### 1.2 Compiler options
Measure the performance achievable with combinations of the following options "-O1", "-O2", "-O3", and "-Ofast" and "-xhost" (change CFLAGS in Makefile for that). In addition, options "-ipo" and "-fno-alias" and "-xCORE-AVX512" are useful. You can also try to use compiler pragmas such as "ivdep" (look it up!). Find the combination of flags that gives you the best performance. Measure on the login node and on the compute nodes. Use the Intel compiler.

* What is the meaning of -ipo? Interprocedural optimization, improves performance in programs with frequently used small or medium function length. I analyzes the entire program
* What is the meaning of -fno-alias? Assumes there is no aliasing, which allows more aggressive optimizations, ex reordering memory access and eliminating unnecessry memory loads/stores
* What is the meaning of "ivdep"? Ignore assumed vector dependencies, it optimizes at compile time when usually would not happen (ex. unknown index n). Different vectorization strategy and checks than SIMD (much slower for relax_jacobi)
* What is the meaning of "-xCORE-AVX512"? Optimizes for SIMD instructions for AVX512 instruction set architecture

The Intel compiler provides reports when using "opt-report" option. Remember that only relax_jacobi.c is relevant for the overall performance. What does it tell you, and what does it mean? Tells how the compiler optimized each loop

Use the following options to trigger opt-report: -qopt-report-annotate -qopt-report-phase=vec,loop. Copy the output for relax_jacobi and explain what it tells you. 
* Is the code vectorized by the compiler? Sometimes, sometimes already vectorized, peels loop (splits)
* What does it mean by -qopt-zmm-usage? This option may provide better code optimization for Intel® processors that are on the Intel® microarchitecture formerly code-named Skylake. (SIMD) ZMM are 512bit wide registers, allow higher parallelism and more efficient processing for large datasets. ZMM: could be instruction or vector register? How are the instructions structured?
* What is the performance result of these options. Present a graph as outlined in the meeting.
* Report on any performance variability you found as presented in the SuperMUC NG presentation.


### 1.3 Batch processing

Run the code as batch job. Does the performance differ to a run on the login node? Give a second graph. Can you think about any reason?
