import matplotlib.pyplot as plt
import numpy as np

########
# DATA #
########
across_nodes = np.array([
    [0, 0.177092],     
    [1, 1.197487],     
    [2, 2.478861],     
    [3, 4.985742],     
    [4, 8.707044],     
    [5, 17.028317],    
    [6, 34.466801],    
    [7, 67.390618],    
    [8, 132.849020],   
    [9, 253.330560],   
    [10, 472.898057],   
    [11, 826.246192],   
    [12, 1340.826960],  
    [13, 1979.719795],  
    [14, 2328.610426],  
    [15, 3372.367333],  
    [16, 2231.885149],  
    [17, 4814.494657],  
    [18, 6912.348460],  
    [19, 8669.347834],  
    [20, 9715.675021],  
    [21, 10109.555271], 
    [22, 10571.748217], 
    [23, 11018.630401], 
    [24, 11505.777960],
])

#########
# PLOTS #
#########

fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(8, 5))

ax1.set_xlabel("Message Size [Byte]")
ax1.set_ylabel("Bandwidth [MByte/s]")
ax1.plot(across_nodes[:,0], across_nodes[:,1], '.-', label="across nodes")
# ax1.set_xscale("log")
ax1.set_xticks(across_nodes[:,0])
xlables = [str(int(i)) for i in across_nodes[:,0]]
ax1.set_xticklabels([r"$2^{" + i + r"}$" for i in xlables])

ax1.set_title("Group 2:  Bandwidth for Communication")
ax1.grid(True, linestyle='--')
ax1.legend()
plt.tight_layout()
plt.savefig("group2_bandwidth.png", dpi=200)
plt.show()
plt.close()
