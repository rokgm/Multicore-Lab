import numpy as np
import matplotlib.pyplot as plt

depths = np.array([2, 3, 4, 5])

# evalso_start = np.array([645, ])
# evalso_mid1 = np.array([545])
# evalso_mid2 = np.array([])
# evalso_end = np.array([])

# leaveso_start = np.array([1936, ])
# leaveso_mid1 = np.array([6965])
# leaveso_mid2 = np.array([4036])
# leaveso_end = np.array([])

# nodeso_start = np.array([45, ])
# nodeso_mid1 = np.array([89])
# nodeso_mid2 = np.array([])
# nodeso_end = np.array([])

# timeso_start = np.array([0.003, ])
# timeso_mid1 = np.array([0.012])
# timeso_mid2 = np.array([])
# timeso_end = np.array([])

# evalsx_start = np.array([561, ])
# evalsx_mid1 = np.array([538])
# evalsx_mid2 = np.array([])
# evalsx_end = np.array([])

# leavesx_start = np.array([2244, ])
# leavesx_mid1 = np.array([5918])
# leavesx_mid2 = np.array([])
# leavesx_end = np.array([])

# nodesx_start = np.array([45, ])
# nodesx_mid1 = np.array([72])
# nodesx_mid2 = np.array([])
# nodesx_end = np.array([])

# timesx_start = np.array([0.004, ])
# timesx_mid1 = np.array([0.011, ])
# timesx_mid2 = np.array([])
# timesx_end = np.array([])

evalrate_start = np.array([645, 686, 709, 673])
evalrate_mid1 = np.array([545, 553, 562, 584])
evalrate_mid2 = np.array([672, 666, 679, 685])
evalrate_end = np.array([616, 692, 649, 736]) 

fig, ax1 = plt.subplots()

ax1.set_xlabel("Search depth", fontsize=13)
ax1.set_ylabel("Evaluations per second (thousands)", fontsize=13)
ax1.set_title("Group 2: Evaluations per second for sequential negamax")
# ax1.set_xlim([0,49])
# ax1.set_ylim([0,30])
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