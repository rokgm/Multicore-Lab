import numpy as np
import matplotlib.pyplot as plt

n_threads = np.array([1, 2, 4, 8, 16, 32, 48])

runtime_seq2000 = 0.251787
runtime_seq6000 = 2.505904

runtime_par2000_scatter = np.array([0.244747, 0.116606, 0.068566, 0.030667, 0.017223, 0.011598, 0.012400])
runtime_par6000_scatter = np.array([2.556724, 1.268657, 0.653841, 0.346104, 0.241116, 0.220410, 0.242361])

runtime_par2000_compact = np.array([0.246151, 0.215263, 0.118153, 0.064421, 0.038067, 0.026543, 0.024379])
runtime_par6000_compact = np.array([2.554119, 2.246860, 1.161415, 0.614099, 0.448881, 0.401189, 0.415308])

speedups2000_scatter = runtime_seq2000 / runtime_par2000_scatter
speedups6000_scatter = runtime_seq6000 / runtime_par6000_scatter

speedups2000_compact = runtime_seq2000 / runtime_par2000_compact
speedups6000_compact = runtime_seq6000 / runtime_par6000_compact

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
ax1.set_title("Group 2: Speedup using OpenMP, with changed KMP affinity settings")
ax1.set_xlim([0,49])
ax1.set_ylim([0,30])

plt.plot(n_threads, speedups2000_compact, '-bo', label='resolution 2000, compact affinity')
plt.plot(n_threads, speedups6000_compact, '-ro', label='resolution 6000, compact affinity')
plt.plot(n_threads, speedups2000_scatter, '--bo', label='resolution 2000, scatter affinity')
plt.plot(n_threads, speedups6000_scatter, '--ro', label='resolution 6000, scatter affinity')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()