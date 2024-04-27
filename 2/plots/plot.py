import matplotlib.pyplot as plt
import numpy as np

########
# DATA #
########
resolutions = [500, 1500, 2500, 3500, 4500]

### -O2
O2_L2_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [0.0263, 0.0264, 0.0304, 0.0286, 0.0273], 
    [0.0262, 0.0263, 0.0304, 0.0286, 0.0273], 
    [0.0263, 0.0263, 0.0303, 0.0285, 0.0273], 
    [0.0263, 0.0264, 0.0304, 0.0286, 0.0273], 
    [0.0263, 0.0264, 0.0304, 0.0286, 0.0273]
])

O2_L3_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [8.370136e-06, 0.0013, 0.0046, 0.0058, 0.0051], 
    [1.277794e-05, 0.0009, 0.0046, 0.0058, 0.0051], 
    [1.020539e-05, 0.0014, 0.0047, 0.0058, 0.0051], 
    [1.072460e-05, 0.0022, 0.0046, 0.0058, 0.0051], 
    [1.114209e-05, 0.0010, 0.0046, 0.0058, 0.0051]
])

O2_MFLOPS_5runs = np.array([
    [1148.3606, 911.0971, 612.2327, 577.3496, 576.9773], 
    [1152.4873, 918.3737, 613.9855, 575.6245, 575.8973], 
    [1155.6224, 927.8827, 610.2758, 574.3577, 576.2277], 
    [1155.1886, 831.3845, 615.1230, 574.1207, 576.5454], 
    [1149.1087, 941.1531, 612.3313, 574.9093, 575.6708]
])

### -O3 -fno-alias -xhost
O3_L2_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [0.0858, 0.0897, 0.1004, 0.1008, 0.1012],
    [0.0857, 0.0896, 0.1004, 0.1008, 0.1012],
    [0.0857, 0.0896, 0.1004, 0.1008, 0.1012],
    [0.0857, 0.0897, 0.1004, 0.1008, 0.1012],
    [0.0857, 0.0897, 0.1005, 0.1008, 0.1012]
])

O3_L3_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [3.823076e-05, 0.0014, 0.0010, 0.0017, 0.0017],
    [4.495033e-05, 0.0011, 0.0008, 0.0014, 0.0012],
    [3.866456e-05, 0.0024, 0.0011, 0.0016, 0.0014],
    [4.781330e-05, 0.0018, 0.0010, 0.0013, 0.0013],
    [4.953515e-05, 0.0027, 0.0012, 0.0013, 0.0015]
])

# Number is for DP, for AVX DP the number is equal +/- 5.
O3_MFLOPS_5runs = np.array([
    [3666.4997, 4414.6386, 3074.1369, 2805.9896, 2786.7321], 
    [3754.0356, 4114.5924, 3055.8120, 2799.1036, 2782.2887],
    [3754.1325, 4443.6424, 3061.5242, 2801.0804, 2793.6859],
    [3770.3622, 3885.4006, 3067.7480, 2809.9452, 2787.5355],
    [3791.7184, 4102.2558, 3003.3868, 2785.2065, 2782.3591]
])

#########
# PLOTS #
#########

# O3 ##############################
fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))

ax1.set_xlabel("Resolution", fontsize=13)

ax1.set_ylabel("MFlops", fontsize=13)
mflops_plot = ax1.errorbar(resolutions, np.average(O3_MFLOPS_5runs, axis=0),
             yerr=np.std(O3_MFLOPS_5runs, axis=0),
             capsize=5, fmt='.-', color='red', label="MFlops (DP/DP AVX)")

# Set shared axis
ax2 = ax1.twinx()
ax2.set_ylabel("L2/L3 miss rate in %", fontsize=13)

L2_plot = ax2.errorbar(resolutions, np.average(O3_L2_miss_rate_5runs, axis=0),
             yerr=np.std(O3_L2_miss_rate_5runs, axis=0),
             capsize=5, fmt='.-', color='green', label="L2 miss rate")

L3_plot = ax2.errorbar(resolutions, np.average(O3_L3_miss_rate_5runs, axis=0),
             yerr=np.std(O3_L3_miss_rate_5runs, axis=0),
             capsize=5, fmt='.-', color='blue', label="L3 miss rate")

ax1.set_title("Group 2: LIKWID Analysis Averaged over 5 Runs with Std. Deviations\nCompiler Flags: -O3 -fno-alias -xhost\n")
ax1.grid(True, linestyle='-.')
ax1.legend(handles=[mflops_plot, L2_plot, L3_plot], framealpha=1,
           bbox_to_anchor=(1, 0.7))
plt.tight_layout()
plt.savefig("2/plots/best.png", dpi=200)
# plt.show()
plt.close()


# O2 ##############################
fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))

ax1.set_xlabel("Resolution", fontsize=13)

ax1.set_ylabel("MFlops", fontsize=13)
mflops_plot = ax1.errorbar(resolutions, np.average(O2_MFLOPS_5runs, axis=0),
             yerr=np.std(O2_MFLOPS_5runs, axis=0),
             capsize=5, fmt='.-', color='red', label="MFlops (DP)")

# Set shared axis
ax2 = ax1.twinx()
ax2.set_ylabel("L2/L3 miss rate in %", fontsize=13)

L2_plot = ax2.errorbar(resolutions, np.average(O2_L2_miss_rate_5runs, axis=0),
             yerr=np.std(O2_L2_miss_rate_5runs, axis=0),
             capsize=5, fmt='.-', color='green', label="L2 miss rate")

L3_plot = ax2.errorbar(resolutions, np.average(O2_L3_miss_rate_5runs, axis=0),
             yerr=np.std(O2_L3_miss_rate_5runs, axis=0),
             capsize=5, fmt='.-', color='blue', label="L3 miss rate")


ax1.set_title("Group 2: LIKWID Analysis Averaged over 5 Runs with Std. Deviations\nCompiler Flags: -O2\n")
ax1.grid(True, linestyle='-.')
ax1.legend(handles=[mflops_plot, L2_plot, L3_plot], framealpha=1,
           bbox_to_anchor=(1, 0.7))
plt.tight_layout()
plt.savefig("2/plots/O2.png", dpi=200)
# plt.show()
plt.close()
