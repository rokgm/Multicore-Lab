import numpy as np
import matplotlib.pyplot as plt

n_threads = np.array([1, 2, 4, 8, 16, 32, 48])

runtime_seq2000 = 0.251787
runtime_seq6000 = 2.505904

runtime_par2000 = np.array([0.377070, 0.184560, 0.108105, 0.070722, 0.063230, 0.034620, 0.027356])
runtime_par6000 = np.array([2.556085, 1.296653, 0.691287, 0.472511, 0.402844, 0.416156, 0.501196])

runtime_par2000_1sttouch = np.array([0.250159, 0.114995, 0.062939, 0.031924, 0.016566, 0.011506, 0.012294])
runtime_par6000_1sttouch = np.array([2.509825, 1.245618, 0.648322, 0.343418, 0.239236, 0.220098, 0.243213])


speedups2000 = runtime_seq2000 / runtime_par2000
speedups6000 = runtime_seq6000 / runtime_par6000

speedups2000_1sttouch = runtime_seq2000 / runtime_par2000_1sttouch
speedups6000_1sttouch = runtime_seq6000 / runtime_par6000_1sttouch

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
ax1.set_title("Group 2: Speedup using OpenMP")
ax1.set_xlim([0,49])
ax1.set_ylim([0,30])

plt.plot(n_threads, speedups2000, '-bo', label='resolution 2000')
plt.plot(n_threads, speedups6000, '-ro', label='resolution 6000')
plt.plot(n_threads, speedups2000_1sttouch, '--bo', label='resolution 2000 with first touch')
plt.plot(n_threads, speedups6000_1sttouch, '--ro', label='resolution 6000 with first touch')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()