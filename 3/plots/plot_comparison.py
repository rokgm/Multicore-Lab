import numpy as np
import matplotlib.pyplot as plt


resolutions = np.array([500, 1500, 2500, 3500, 4500])


# Baseline performance
orig_runtime =      np.array([1.0, 2.0, 3.0, 4.0, 5.0]) # s
orig_L2missrate =   np.array([20, 19, 18, 17, 16]) # %
orig_L3missrate =   np.array([30, 29, 28, 27, 26]) # %

# Our best performance
tuned_runtime =     np.array([0.5, 1.5, 2.0, 3.1, 6.2])
tuned_L2missrate =  np.array([15, 15, 15, 15, 15])
tuned_L3missrate =  np.array([5, 4, 3, 2, 1])

# Change
perc_runtime = tuned_runtime / orig_runtime * 100
perc_L2missrate = tuned_L2missrate / orig_L2missrate * 100
perc_L3missrate = tuned_L3missrate / orig_L3missrate * 100

perc_data = {
    'Runtime': perc_runtime,
    'L2 missrate': perc_L2missrate,
    'L3 missrate': perc_L3missrate,
}

fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(7, 4))

bar_resolutions = ['500', '1500', '2500', '3500', '4500'] # Needs to be text or the bar plot comes out stupid

x = np.arange(len(bar_resolutions))  # the label locations
width = 0.25  # the width of the bars
multiplier = 0

for attribute, measurement in perc_data.items():
    offset = width * multiplier
    rects = ax1.bar(x + offset, measurement, width, label=attribute)
    ax1.bar_label(rects, padding=3)
    multiplier += 1


# ax1.bar(bar_resolutions, perc_runtime)

ax1.axhline(y = 100, color = 'r', linestyle = '-')


ax1.set_xlabel("Resolution", fontsize=13)
ax1.set_ylabel("Performance vs baseline [%]", fontsize=13)
ax1.set_title("Group 2: Performance of tuned code compared to baseline")
ax1.set_xticks(x + width, bar_resolutions)
ax1.legend(loc='upper left', ncols=3)
plt.tight_layout()
plt.show()

