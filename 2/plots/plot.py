import matplotlib.pyplot as plt
import numpy as np

########
# DATA #
########
resolutions = [500, 1500, 2500, 3500, 4500]
### -O2

### -O3 -fno-alias -xhost
L2_miss_ratio_5runs = 100 * np.array([      # in percents
    [0.2364, 0.1819, 0.1923, 0.1898, 0.1953], 
    [0.2374, 0.1820, 0.1920, 0.1893, 0.1954],
    [0.2366, 0.1820, 0.1918, 0.1892, 0.1952],
    [0.2365, 0.1815, 0.1916, 0.1895, 0.1952],
    [0.2379, 0.1818, 0.1917, 0.1895, 0.1952]
])

L3_miss_ratio_5runs = 100 * np.array([      # in percents
    [0.0001, 0.4309, 0.7725, 0.9224, 0.9392], 
    [0.0002, 0.4227, 0.7862, 0.9211, 0.9438], 
    [0.0001, 0.4658, 0.7800, 0.9255, 0.9393],
    [0.0002, 0.3932, 0.7832, 0.9270, 0.9384], 
    [0.0002, 0.4096, 0.7916, 0.9249, 0.9373]
])

# Number is for DP, for AVX DP the number is equal +/- 5.
MFLOPS_5runs = np.array([
    [1776.9151, 1627.7634, 1469.7033, 1330.5803, 1320.5122],
    [1778.2934, 1554.1183, 1495.4391, 1316.9242, 1322.0736],
    [1873.5473, 1668.4818, 1479.6065, 1305.5911, 1322.8326],
    [1890.2021, 1597.1130, 1502.2886, 1322.8273, 1321.8052], 
    [1755.8812, 1552.3627, 1464.1460, 1329.8544, 1326.3065]
])

#########
# PLOTS #
#########

fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))

ax1.set_xlabel("Resolution", fontsize=13)

ax1.set_ylabel("MFlops", fontsize=13)
mflops_plot = ax1.errorbar(resolutions, np.average(MFLOPS_5runs, axis=0),
             yerr=np.std(MFLOPS_5runs, axis=0),
             capsize=5, fmt='.-', color='blue', label="MFlops")

# Set shared axis
ax2 = ax1.twinx()
ax2.set_ylabel("L2/L3 miss ratio in %", fontsize=13)

L2_plot = ax2.errorbar(resolutions, np.average(L2_miss_ratio_5runs, axis=0),
             yerr=np.std(L2_miss_ratio_5runs, axis=0),
             capsize=5, fmt='.-', color='red', label="L2 miss ratio")

L3_plot = ax2.errorbar(resolutions, np.average(L3_miss_ratio_5runs, axis=0),
             yerr=np.std(L3_miss_ratio_5runs, axis=0),
             capsize=5, fmt='.-', color='orange', label="L3 miss ratio")

ax1.set_title("Group 2: LIKWID Analysis averaged over 5 runs\nCompiler Flags: -O3 -fno-alias -xhost\n")
ax1.legend(handles=[mflops_plot, L2_plot, L3_plot], frameon=False,
           bbox_to_anchor=(1, 0.7))
plt.tight_layout()
plt.savefig("best.png")
plt.show()
