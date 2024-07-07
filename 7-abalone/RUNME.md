To compile the code run the following command:
```make all```

To run the player instance with Parallel Alpha-Beta strategy (number 5) run the following command:
```
export OMP_NUM_THREADS=48
export KMP_AFFINITY=granularity=fine,scatter
./player -s 5 1000 X
```
or for the opposite color:
``` ./player -s 5 1000 O ```
We set max depth to 1000, as we will limit by time and not depth.