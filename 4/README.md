## OpenMP

For all further assignments, please use the provided optimized code taken from ASSIGNMENT 4.1 (see above).

**ASSIGNMENT 4.2**: The Intel compiler supports automatic parallelization. It is switched on with the "-parallel" option. The compiler tries to parallelize simple loops. Please read the compiler documentation available.

**TASK 4.2.1**: Try the automatic parallelization feature of icc. The compiler might parallelize all the loops. With the "-opt-report-phase=par" option you will receive a report explaining the reasons why parallelization was not possible. Improve the parallelization by rewriting the code and inserting pragmas (no OpenMP pragmas). Be able to explain why it worked or did not work in a short report.

**TASK 4.2.2**: Do performance measurements for 1, 2, 4, 8, 12, 16, 24, 32 and 48 threads on SuperMUC with the configuration in test.dat. Provide a speedup graph for the given configurations. In a speedup graph the x-axis is the number of threads and the y-axis the achieved speedup. Use the reported execution time for the given sequential code as the basis for the speedup calculation. Use a linear scale for x- and y-axis.

![moodle graph](image.png)

**TASK 4.2.3**: Is there a difference between the sequential time from Assignment 4.1 and the sequential time of automatically parallelized version?

#### Deliverable
Provide a speedup graph for the given configurations where the x-axis is the number of threads and the y-axis the achieved speedup. Use the reported execution time for the sequential code (Assignment 4.1) as the basis for the speedup calculation. (File name: GroupN_ASS4.2.png)