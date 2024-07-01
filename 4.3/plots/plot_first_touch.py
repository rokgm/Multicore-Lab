import numpy as np
import matplotlib.pyplot as plt

n_threads = np.array([1, 2, 4, 8, 12, 16, 24, 32, 48])

runtime_seq2000 = 0.251787
runtime_seq6000 = 2.505904

# runtime_static2000 = np.array([0.245332, 0.138423, 0.077038, 0.043875, 0.026543, 0.009723, 0.009514])
runtime_static2000 = np.array([0.245332, 0.138423, 0.077038, 0.043875, 0.032873, 0.026543, 0.020651, 0.009723, 0.009514])
# runtime_static6000 = np.array([2.551647, 1.313590, 0.688382, 0.478225, 0.403553, 0.303614, 0.200333])
runtime_static6000 = np.array([2.551647, 1.313590, 0.688382, 0.478225, 0.425190, 0.403553, 24, 0.303614, 0.200333])

runtime_dynamic2000 = np.array([0.243776, 0.180937, 0.108715, 0.070887, 12, 0.045624, 24, 0.080339, 0.074116])
runtime_dynamic6000 = np.array([2.544038, 1.527977, 0.871051, 0.569373, 12, 0.494432, 24, 0.911072, 0.834821])


speedups2000_static = runtime_seq2000 / runtime_static2000
speedups6000_static = runtime_seq6000 / runtime_static6000

speedups2000_dynamic = runtime_seq2000 / runtime_dynamic2000
speedups6000_dynamic = runtime_seq6000 / runtime_dynamic6000

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
ax1.set_title("Group 2: Speedup using static or dynamic scheduling")
ax1.set_xlim([0,49])
ax1.set_ylim([0,30])

plt.plot(n_threads, speedups2000_static, '-bo', label='resolution 2000 with first touch (cyclic)')
plt.plot(n_threads, speedups6000_static, '-ro', label='resolution 6000 with first touch (cyclic)')
plt.plot(n_threads, speedups2000_dynamic, '--bo', label='resolution 2000 with first touch (block)')
plt.plot(n_threads, speedups6000_dynamic, '--ro', label='resolution 6000 with first touch (block)')
plt.legend()
plt.grid()
plt.tight_layout()
# plt.show()
plt.savefig('Group2_ASS4.3_first_touch.png')