import numpy as np
import matplotlib.pyplot as plt

threads = np.array([1, 2, 4, 8, 12, 16, 24, 32, 48])

seq_start = 709
seq_mid1 = 562
seq_mid2 = 679
seq_end = 649

evalrate_start = np.array([689, 940, 1171, 1991, 2232, 2330, 2409, 1402, 1458])
evalrate_mid1 = np.array([604, 815, 1007, 1758, 2129, 2263, 2348, 1397, 1339])
evalrate_mid2 = np.array([688, 796, 1208, 1887, 2250, 2350, 2437, 1453, 1477]) 
evalrate_end = np.array([639, 867, 1142, 1846, 2211, 2372, 2304, 1354, 1448])

speedups_start = evalrate_start / seq_start
speedups_mid1 = evalrate_mid1 / seq_mid1
speedups_mid2 = evalrate_mid2 / seq_mid2
speedups_end = evalrate_end / seq_end
# print(speedups_end)

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup based on eval/s", fontsize=13)
ax1.set_title("Group 2: Speedup of parallel negamax for depth 4")

# plt.xticks(range(1,6))

plt.plot(threads, speedups_start, '-bo', label='Default start position')
plt.plot(threads, speedups_mid1, '-ro', label='Midgame position 1')
plt.plot(threads, speedups_mid2, '-go', label='Midgame position 2')
plt.plot(threads, speedups_end, '-mo', label='Endgame position')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()
# plt.savefig('Group2_ASS4.3_first_touch.png')