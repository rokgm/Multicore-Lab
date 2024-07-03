import numpy as np
import matplotlib.pyplot as plt
# measurements from new negamax with correct board copy
# all from start board position and depth 4

threads = np.array([1, 2, 4, 8, 12, 16, 24, 32, 48])
threads_2 = np.array([24, 32, 48])
seq_start = 709

evalrate_std = np.array([580, 897, 1172, 1734, 1988, 2146, 2266, 1399, 1397])
evalrate_compact_fine = np.array([624, 652, 932, 1365, 1557, 1953, 2498, 2402, 2550])
# evaluate_scatetr_fine = np.array([])
evaluate_compact_socket = np.array([ 2233, 2381, 2556])
evaluate_compact_core = np.array([624, 652, 921, 1385, 1601, 1881, 2548, 2391, 2574])


speedup_std =  evalrate_std / seq_start
speedup_compact_fine = evalrate_compact_fine /seq_start
speedup_compact_socket = evaluate_compact_socket /seq_start
speedup_compact_core = evaluate_compact_core /seq_start

fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of threads", fontsize=13)
ax1.set_ylabel("Speedup based on eval/s", fontsize=13)
ax1.set_title("Evals/s speedup for parallelized negamax at different KMP affinities")
# ax1.set_xlim([0,49])
# ax1.set_ylim([0,30])
# plt.xticks(range(2,6))

plt.plot(threads, speedup_std, '-bo', label='Core respect (default)')
plt.plot(threads, speedup_compact_fine, '-ro', label='Fine compact')
# plt.plot(threads_2, speedup_compact_socket, '-go', label='Socket compact')
# plt.plot(threads, speedup_compact_core, '-yo', label='Core compact')
plt.legend()
# plt.grid()
plt.tight_layout()
plt.show()
# plt.savefig('Group2_ASS4.3_first_touch.png')