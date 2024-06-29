import numpy as np
from itertools import groupby
import matplotlib.pyplot as plt
import os

# Sequential runtimes
runtime_seq2000 = 0.251787
runtime_seq6000 = 2.505904

speedups = {}
runtimes = {}
ncores = {}

# Directory containing the .txt files
dir_path = './6/hybrid/measurements'

# Loop over all files in the directory
for filename in os.listdir(dir_path):
    # Check if the file ends with ".out"
    if filename.endswith('.out'):
        # Full path to the file
        filepath = os.path.join(dir_path, filename)

        # Open the file
        with open(filepath, 'r') as file:
            # Read the file and split it into lines
            temp2000 = []
            temp6000 = []
            for i, line in enumerate(file):
                # Check if the line number is one of the ones you're interested in
                if i in [9, 32, 55, 78, 101]:
                    # Split the line on the colon and take the second part, which is the runtime
                    runtime2000 = float(line.split(':')[1].strip())
                    # Add the runtime to the dictionary
                    temp2000.append(runtime2000)
                if i in [18, 41, 64, 87, 110]:
                    # Split the line on the colon and take the second part, which is the runtime
                    runtime6000 = float(line.split(':')[1].strip())
                    # Add the runtime to the dictionary
                    temp6000.append(runtime6000)
            
            average_runtime2000 = sum(temp2000) / len(temp2000)

            # Extract the runtimes and calculate the speedups
            key2000 = filename.split('.')[0] + ':2000'
            runtimes[key2000] = average_runtime2000
            speedups[key2000] = runtime_seq2000 / runtimes[key2000]
            ncores[key2000] = int(filename.split('=')[1].split('_')[0]) * 48
            
            average_runtime6000 = sum(temp6000) / len(temp6000)

            # Extract the runtimes and calculate the speedups
            key6000 = filename.split('.')[0] + ':6000'
            runtimes[key6000] = average_runtime6000
            speedups[key6000] = runtime_seq6000 / runtimes[key6000]
            ncores[key6000] = int(filename.split('=')[1].split('_')[0]) * 48

# Print the speedups and runtimes
# for key in speedups:
#     print(key, speedups[key], runtimes[key], ncores[key])


fig = plt.figure()
ax1 = fig.add_subplot(111)

# Get unique ncores values and sort them
unique_ncores = sorted(set(ncores.values()))

# Create a list of keys sorted by ncores and then by key
keys_sorted = sorted(speedups.keys(), key=lambda x: (ncores[x], x))

# Initialize the dictionary with an empty list
speedups_data = {'1D in y, 2000': [], '1D in y, 6000': [], '1D in x, 2000': [], '1D in x, 6000': [], '2D, 2000': [], '2D, 6000': []}

# Loop over the sorted keys
for key in keys_sorted:
    # Split the key on 'x' and check if the second part starts with a non-1 digit
    if key.split('x')[0] == '1' and key.split('x')[1][0] != '1' :
        if key.split(':')[1] == '2000':
            # If it does, add the speedup to the list
            speedups_data['1D in y, 2000'].append(np.round(speedups[key], 2))
        if key.split(':')[1] == '6000':
            # If it does, add the speedup to the list
            speedups_data['1D in y, 6000'].append(np.round(speedups[key], 2))
    elif key.split('x')[0] != '1' and key.split('x')[1][0] == '1':
        if key.split(':')[1] == '2000':
            # If it does, add the speedup to the list
            speedups_data['1D in x, 2000'].append(np.round(speedups[key], 2))
        if key.split(':')[1] == '6000':
            # If it does, add the speedup to the list
            speedups_data['1D in x, 6000'].append(np.round(speedups[key], 2))
    elif key.split('x')[0] != '1' and key.split('x')[1][0] != '1':
        if key.split(':')[1] == '2000':
            # If it does, add the speedup to the list
            speedups_data['2D, 2000'].append(np.round(speedups[key], 2))
        if key.split(':')[1] == '6000':
            # If it does, add the speedup to the list
            speedups_data['2D, 6000'].append(np.round(speedups[key], 2))

bar_resolutions = ['96', '144', '192'] 

print(speedups)
print(speedups_data)

x = np.arange(len(bar_resolutions))  # the label locations
width = 0.1  # the width of the bars
multiplier = 0


for attribute, measurement in speedups_data.items():
    offset = width * multiplier
    rects = ax1.bar(x + offset, measurement, width, label=attribute)
    ax1.bar_label(rects, padding=3)
    multiplier += 1

ax1.set_xlabel("Number of cores", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
# ax1.set_title("Runtime of tuned code \ncompared to baseline")
ax1.set_xticks(x + width, bar_resolutions)
ax1.legend(loc='best')

plt.show()


