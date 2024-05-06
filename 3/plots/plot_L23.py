import numpy as np
import matplotlib.pyplot as plt


# Baseline performance
orig_L2missratio =   np.array([0.2362, 0.1878, 0.1713, 0.1705, 0.1741]) # %
orig_L3missratio =   np.array([0.0016, 0.3937, 0.8039, 0.8943, 0.9545]) # %
orig_L2hit = np.array([26598030, 299879600, 913084200, 1797342000, 2909023000]) # L2_TRANS_ALL_REQUESTS
orig_L2miss = np.array([6282041, 56318080, 156413500, 306491800, 506600600]) # L2_RQSTS_MISS
orig_L3hit = np.array([1043408, 2717900, 471536, 767392, 432864]) # MEM_LOAD_RETIRED_L3_HIT
orig_L3miss = np.array([1718, 1764884, 1933281, 6492846, 9072937]) # MEM_LOAD_RETIRED_L3_MISS

# Our best performance
tuned_L2missratio =  np.array([0.2178, 0.1532, 0.1524, 0.1521, 0.1630])
tuned_L3missratio =  np.array([0.0061, 0.7647, 0.9573, 0.9769, 0.9890])
tuned_L2hit = np.array([11588610, 146670200, 410185200, 806053400, 1242937000])
tuned_L2miss = np.array([2523790, 22477110, 62511710, 122576200, 202596300])
tuned_L3hit = np.array([8310, 50581, 160141, 218416, 105926])
tuned_L3miss = np.array([51, 164384, 3587980, 9243156, 9505799])


perc_data = {
    'L2 hit baseline': np.round(orig_L2hit, 2),
    'L2 hit': np.round(tuned_L2hit, 2),
    'L2 missb baseline': np.round(orig_L2miss, 2),
    'L2 miss': np.round(tuned_L2miss, 2),
    'L3 hit baseline': np.round(orig_L3hit, 2),
    'L3 hit': np.round(tuned_L3miss, 2),
    'L3 miss baseline': np.round(orig_L3miss, 2),
    'L3 miss': np.round(tuned_L3miss, 2),
}

# fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))
fig, ax1 = plt.subplots()

bar_resolutions = ['500', '1500', '2500', '3500', '4500'] # Needs to be text or the bar plot comes out stupid

x = np.arange(len(bar_resolutions))  # the label locations
width = 0.25  # the width of the bars
multiplier = 0

for attribute, measurement in perc_data.items():
    offset = width * multiplier
    rects = ax1.bar(x + offset, measurement, width, label=attribute)
    # ax1.bar_label(rects, padding=3)
    multiplier += 1


# ax1.bar(bar_resolutions, perc_runtime)

# ax1.axhline(y = 100, color = 'r', linestyle = '-')


ax1.set_xlabel("Resolution", fontsize=13)
ax1.set_ylabel("N", fontsize=13)
ax1.set_title("Group 2: Performance of tuned code compared to baseline")
ax1.set_xticks(x + width, bar_resolutions)
ax1.legend(loc='upper right')
# ax1.legend(loc='upper right', ncols=3)
plt.tight_layout()
plt.show()

