import numpy as np
import matplotlib.pyplot as plt


resolutions = np.array([500, 1500, 2500, 3500, 4500])


# Baseline performance
orig_runtime =       np.array([0.032248, 0.234791, 0.911041, 1.910234, 3.167433]) # s
orig_L2missratio =   np.array([0.2362, 0.1878, 0.1713, 0.1705, 0.1741]) # %
orig_L3missratio =   np.array([0.0016, 0.3937, 0.8039, 0.8943, 0.9545]) # %

# Our best performance
tuned_runtime =      np.array([0.015493, 0.116756, 0.357297, 0.722641, 1.212902])
tuned_L2missratio =  np.array([0.2178, 0.1532, 0.1524, 0.1521, 0.1630])
tuned_L3missratio =  np.array([0.0061, 0.7647, 0.9573, 0.9769, 0.9890])

# Change
perc_runtime = tuned_runtime / orig_runtime * 100
perc_L2missratio = tuned_L2missratio / orig_L2missratio * 100
perc_L3missratio = tuned_L3missratio / orig_L3missratio * 100

perc_data = {
    'Runtime': np.round(perc_runtime, 2),
    # 'L2 missratio': np.round(perc_L2missratio, 2),
    # 'L3 missratio': np.round(perc_L3missratio, 2),
}

# fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))
fig, ax1 = plt.subplots()

bar_resolutions = ['500', '1500', '2500', '3500', '4500'] # Needs to be text or the bar plot comes out stupid

x = np.arange(len(bar_resolutions))  # the label locations
width = 0.25  # the width of the bars
multiplier = 0

for attribute, measurement in perc_data.items():
    # offset = width * multiplier
    offset=0
    rects = ax1.bar(x, measurement, width, label=attribute)
    ax1.bar_label(rects, padding=3)
    multiplier += 1


# ax1.bar(bar_resolutions, perc_runtime)

ax1.axhline(y = 100, color = 'r', linestyle = '-')


ax1.set_xlabel("Resolution", fontsize=13)
ax1.set_ylabel("Performance vs baseline [%]", fontsize=13)
ax1.set_title("Performance of tuned code compared to baseline")
ax1.set_xticks(x, bar_resolutions)
# ax1.legend(loc='upper right', ncols=3)
ax1.legend(loc='best')
plt.tight_layout()
plt.show()

