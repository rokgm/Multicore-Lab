import numpy as np
import matplotlib.pyplot as plt

n_threads = np.array([1, 2, 4, 8, 16, 32, 48])

runtime_seq2000 = 0.251787
runtime_seq6000 = 2.505904

runtime_par2000 = np.array([0.377070, 0.184560, 0.108105, 0.070722, 0.063230, 0.034620, 0.027356])
runtime_par6000 = np.array([2.556085, 1.296653, 0.691287, 0.472511, 0.402844, 0.416156, 0.501196])

# runtime_par2000_1sttouch = np.array([0.258587, 0.137389, 0.078328, 0.042737, 0.025993, 0.009824, 0.006439])
# runtime_par6000_1sttouch = np.array([2.570008, 1.312078, 0.691502, 0.472711, 0.406836, 0.303416, 0.200211])


speedups2000 = runtime_seq2000 / runtime_par2000
speedups6000 = runtime_seq6000 / runtime_par6000

# speedups2000_1sttouch = runtime_seq2000 / runtime_par2000_1sttouch
# speedups6000_1sttouch = runtime_seq6000 / runtime_par6000_1sttouch

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
ax1.set_title("Group 2: Speedup using OpenMP")
ax1.set_xlim([0,49])
ax1.set_ylim([0,40])

plt.plot(n_threads, speedups2000, '-bo', label='resolution 2000')
plt.plot(n_threads, speedups6000, '-ro', label='resolution 6000')
# plt.plot(n_threads, speedups2000_1sttouch, '--bo', label='resolution 2000 with first touch')
# plt.plot(n_threads, speedups6000_1sttouch, '--ro', label='resolution 6000 with first touch')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()