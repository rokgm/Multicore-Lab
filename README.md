# Multicore Lab SoSe 2024
Group 2: Tiia Tikkala, Beatrice Picco, Rok Grgič Meško

## Assignment 1

### 1.1 Understanding the code
* Show the initial performance data. The form of the graph was introduced in the meeting.
* Explain how the FLOP/s metric is measured. Which floating point operations are taken into account?

### 1.2 Compiler options
Measure the performance achievable with combinations of the following options "-O1", "-O2", "-O3", and "-Ofast" and "-xhost" (change CFLAGS in Makefile for that). In addition, options "-ipo" and "-fno-alias" and "-xCORE-AVX512" are useful. You can also try to use compiler pragmas such as "ivdep" (look it up!). Find the combination of flags that gives you the best performance. Measure on the login node and on the compute nodes. Use the Intel compiler.

* What is the meaning of -ipo?
* What is the meaning of -fno-alias?
* What is the meaning of "ivdep"?
* What is the meaning of "-xCORE-AVX512"?

The Intel compiler provides reports when using "opt-report" option. Remember that only relax_jacobi.c is relevant for the overall performance. What does it tell you, and what does it mean?

Use the following options to trigger opt-report: -qopt-report-annotate -qopt-report-phase=vec,loop. Copy the output for relax_jacobi and explain what it tells you. 
* Is the code vectorized by the compiler?
* What does it mean by -qopt-zmm-usage?
* What is the performance result of these options. Present a graph as outlined in the meeting.
* Report on any performance variability you found as presented in the SuperMUC NG presentation.

### 1.3 Batch processing
