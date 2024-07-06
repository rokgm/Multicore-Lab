import numpy as np
import matplotlib.pyplot as plt

threads = np.array([1, 2, 4, 8, 12, 16, 24, 32, 48])

seq_start = 2.105 #X
seq_mid1 = 2.005 #X
seq_mid2 = 1.203 #O
seq_end = 0.103 #X


time_start = np.array([2.257, 4.294, 1.756, 5.150, 2.658, 0.653, 0.680, 0.552, 0.773])
time_mid1 = np.array([2.107, 2.157, 0.954, 1.105, 0.553, 0.552, 0.603, 0.453, 0.402])
time_mid2 = np.array([0.603, 0.503, 0.552, 1.025, 0.302, 0.152, 0.151, 0.152, 0.113]) 
time_end = np.array([0.102, 0.102, 0.151, 0.183, 0.253, 0.102, 0.152, 0.051, 0.102])

speedups_start = seq_start / time_start
speedups_mid1 = seq_mid1 / time_mid1
speedups_mid2 = seq_mid2 / time_mid2
speedups_end = seq_end / time_end


fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup based on runtime", fontsize=13)
ax1.set_title("Speedup of parallel alpha-beta for depth 5")

plt.plot(threads, speedups_start, '-bo', label='Default start position')
plt.plot(threads, speedups_mid1, '-ro', label='Midgame position 1')
plt.plot(threads, speedups_mid2, '-go', label='Midgame position 2')
plt.plot(threads, speedups_end, '-mo', label='Endgame position')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()

