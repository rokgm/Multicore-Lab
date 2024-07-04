import numpy as np
import matplotlib.pyplot as plt

depths = np.array([2, 3, 4, 5])

evalrate_start = np.array([371, 569, 600, 572]) * 1000
evalrate_mid1 = np.array([642, 500, 545, 573]) * 1000
evalrate_mid2 = np.array([319, 708, 545, 672]) * 1000
evalrate_end = np.array([185, 598, 666, 545]) * 1000

fig, ax1 = plt.subplots()

ax1.set_xlabel("Search depth", fontsize=13)
ax1.set_ylabel("Evals/s", fontsize=13)
ax1.set_title("Evaluations per second for sequential negamax with alpha-beta pruning")

plt.xticks(range(2,6))

plt.plot(depths, evalrate_start, '-bo', label='Eval rate for default start position')
plt.plot(depths, evalrate_mid1, '-ro', label='Eval rate for midgame position 1')
plt.plot(depths, evalrate_mid2, '-go', label='Eval rate for midgame position 2')
plt.plot(depths, evalrate_end, '-mo', label='Eval rate for endgame position')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()
# plt.savefig('Group2_ASS4.3_first_touch.png')