# Multicore Lab SoSe 2024
Group 2: Tiia Tikkala, Beatrice Picco, Rok Grgič Meško

## Assignment 1

### 1.1 Understanding the code
* ***Show the initial performance data. The form of the graph was introduced in the meeting.***
[Performance data graph](1_heatdir/plots/group2_perf.pdf)

* ***Explain how the FLOP/s metric is measured. Which floating point operations are taken into account?*** The FLOPS are measured only within relax_jacobi.c. In each iteration, 11 flops are performed in the functions residual_jacobi and relax_jacobi. Are there ways to do this automatically? Now the 11 is hardcoded.


### 1.2 Compiler options
Measure the performance achievable with combinations of the following options "-O1", "-O2", "-O3", and "-Ofast" and "-xhost" (change CFLAGS in Makefile for that). In addition, options "-ipo" and "-fno-alias" and "-xCORE-AVX512" are useful. You can also try to use compiler pragmas such as "ivdep" (look it up!). Find the combination of flags that gives you the best performance. Measure on the login node and on the compute nodes. Use the Intel compiler.

* ***What is the meaning of -ipo?*** Interprocedural optimization, improves performance in programs with frequently used small or medium function length. It analyzes the entire program. Goal is to reduce or eliminate duplicate calculations and memory inefficiencies. Examples of optimizations done with the -ipo flag are function inlining and reordering routines for more efficient usage of memory layout.
* ***What is the meaning of -fno-alias?*** Assumes there is no aliasing (i.e. each memory location is only accessed by one entity within the program), which allows more aggressive optimizations, e.g. reordering memory accesses or eliminating unnecessary memory loads/stores.
* ***What is the meaning of "ivdep"?*** Ignore assumed vector dependencies. It is a compiler pragma (so not a compiler flag). It is specified for each loop in a program separately. Normally the compiler avoids vectorization if undefined/illegal behaviour might occur, but specifying #pragma ivdep means the compiler will assume those situations are safe (e.g. loop variable value unknown at compile time). Different vectorization strategy and checks than SIMD (results are much slower for relax_jacobi in our case).
* ***What is the meaning of "-xCORE-AVX512"?*** Optimizes for SIMD instructions for AVX512 instruction set architecture.

***The Intel compiler provides reports when using "opt-report" option. Remember that only relax_jacobi.c is relevant for the overall performance. What does it tell you, and what does it mean? Tells how the compiler optimized each loop***

***Use the following options to trigger opt-report: -qopt-report-annotate -qopt-report-phase=vec,loop. Copy the output for relax_jacobi and explain what it tells you.***
* ***Is the code vectorized by the compiler?*** Sometimes, for example through loop peeling (splits off single iterations at beginning/end of loop and vectorises the rest).
* ***What does it mean by -qopt-zmm-usage?*** This option may provide better code optimization for Intel® processors that are on the Intel® microarchitecture formerly code-named Skylake. (SIMD) ZMM are 512bit wide registers, allow higher parallelism and more efficient processing for large datasets. ZMM: could be instruction or vector register? How are the instructions structured?
* ***What is the performance result of these options. Present a graph as outlined in the meeting.***
* ***Report on any performance variability you found as presented in the SuperMUC NG presentation.***


### 1.3 Batch processing

***Run the code as batch job. Does the performance differ to a run on the login node? Give a second graph. Can you think about any reason?***
