import numpy as np
import matplotlib.pyplot as plt
# measurements from new negamax with correct board copy
# all from start and depth 4

threads = np.array([1, 2, 4, 8, 12, 16, 24, 32, 48])

evalrate_std = np.array([580, 897, 1172, 1734, 1988, 2146, 2266, 1399, 1397])
evaluate_compact_fine = np.array([624, 652, 932, 1365, 1557, 1953, 2498, 2402, 2550])
evaluate_scatetr_fine = np.array([])

fig, ax1 = plt.subplots()

ax1.set_xlabel("Search depth", fontsize=13)
ax1.set_ylabel("Evaluations per second (thousands)", fontsize=13)
ax1.set_title("Evaluations per second for parallelized negamax at different KMP affinities")
# ax1.set_xlim([0,49])
# ax1.set_ylim([0,30])
# plt.xticks(range(2,6))

plt.plot(threads, evalrate_std, '-bo', label='No affinity specified')
plt.plot(threads, evaluate_compact_fine, '-ro', label='Fine compact')
plt.legend()
# plt.grid()
plt.tight_layout()
plt.show()
# plt.savefig('Group2_ASS4.3_first_touch.png')