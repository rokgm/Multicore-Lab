## MPI


### ASSIGNMENT 5.1
Write a program that sends a message from node A to node B and back (a so called ping-pong). Use the blocking send and receive constructs.

Determine

 - the startup time is the time spent to inject and extract a message to and from the network.
    - For large enough message, startup times increase proportional to the message size.
 - the bandwidth (MB/s), that is the size of the message in bytes divided by the time for messages of length 2$^0$, $2^1$ -$2^24$ bytes.
    - Across nodes:
    For large enough message, bandwidth goes to ~11GByte/s, which is consistent with OmniPath Network stated 100 Gbit/s bandwidth.

Synchronize all processes at the beginning by a barrier. To get around measurement inaccuracies, send the message multiple times and measure the aggregated time. 

Perform the measurement for 

 - communication of processes in the same processor,
 - same node but different processors,
 - and for processes on different nodes. 


#### Deliverable:

A single graph with three curves giving the bandwidth for communication on same socket, across sockets, and across nodes. The unit of the y-axis is bandwidth in Mbytes/sec. The unit of the x-axis is length in bytes. The y-axis should use linear scale. The x-axis uses equidistant labels for each experiment.
![moodle graph](image.png)


### ASSIGNMENT 5.2

Implement a programm that performs a reduction, i.e., that sums up all the elements of an array A of length n. 

The elements of A should be initialized to i, i.e., A(i)=i. The global sum should be computed via a tree and should be printed by process 0.

Distribute array A such that each process only allocates space for its local part. The number of processes should be known only at initialization time. You can assume that it is a power of two. You cannot assume that n is divisible by the number of processes.

DO NOT USE THE MPI COLLECTIVE OPERATION.

#### Deliverable:

Deliver the source code of your implementation. Only one file allowed.

