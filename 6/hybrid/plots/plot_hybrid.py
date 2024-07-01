import numpy as np
from itertools import groupby
import matplotlib.pyplot as plt
import os


def read_file(filename):
    # Sequential runtimes
    runtime_seq2000 = 0.251787
    runtime_seq6000 = 2.505904

    dir_path = './6/hybrid/measurements'
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
        # key2000 = filename.split('.')[0] + ':2000'
        # runtimes[key2000] = average_runtime2000
        average_speedup2000 = runtime_seq2000 / average_runtime2000
        # speedups[key2000] = runtime_seq2000 / runtimes[key2000]
        # ncores[key2000] = int(filename.split('=')[1].split('_')[0]) * 48
        ncores = int(filename.split('=')[1].split('_')[0]) * 48
        
        average_runtime6000 = sum(temp6000) / len(temp6000)

        # Extract the runtimes and calculate the speedups
        # key6000 = filename.split('.')[0] + ':6000'
        # runtimes[key6000] = average_runtime6000
        average_speedup6000 = runtime_seq6000 / average_runtime6000
        # speedups[key6000] = runtime_seq6000 / runtimes[key6000]
        # ncores[key6000] = int(filename.split('=')[1].split('_')[0]) * 48

    # # Print the speedups and runtimes
    # for key in speedups:
    #     print(key, speedups[key], runtimes[key], ncores[key])
    return average_speedup2000, average_speedup6000, ncores


speedups_2000_1d_y = [0]*3
speedups_6000_1d_y = [0]*3
speedups_2000_1d_x = [0]*3
speedups_6000_1d_x = [0]*3

speedups_2000_2d = [0]*3
speedups_6000_2d = [0]*3

ncores_1d = [0]*3
ncores_2d = [0]*3


speedups_2000_1d_x[0], speedups_6000_1d_x[0], ncores_1d[0] = read_file("2x1_nodes=2_ompthreads=48.out")
speedups_2000_1d_x[1], speedups_6000_1d_x[1], ncores_1d[1] = read_file("3x1_nodes=3_ompthreads=48.out")
speedups_2000_1d_x[2], speedups_6000_1d_x[2], ncores_1d[2] = read_file("4x1_nodes=4_ompthreads=48.out")

speedups_2000_1d_y[0], speedups_6000_1d_y[0], ncores_1d[0] = read_file("1x2_nodes=2_ompthreads=48.out")
speedups_2000_1d_y[1], speedups_6000_1d_y[1], ncores_1d[1] = read_file("1x3_nodes=3_ompthreads=48.out")
speedups_2000_1d_y[2], speedups_6000_1d_y[2], ncores_1d[2] = read_file("1x4_nodes=4_ompthreads=48.out")

speedups_2000_2d[0], speedups_6000_2d[0], ncores_2d[0] = read_file("2x2_nodes=4_ompthreads=48.out")


bar_resolutions = ['96', '144', '192'] 
bar_resolutions_fl = [96, 144, 192] 
x = np.arange(len(bar_resolutions))  # the label locations
width = 0.1  # the width of the bars
multiplier = 0


# fig = plt.figure(figsize=[50,20])
# ax1 = fig.add_subplot(111)
fig, ax1 = plt.subplots()

# ax1.bar(list(map(lambda x: str(x - (width*1)), bar_resolutions_fl)), speedups_2000_1d_y, width, label="trial")
# ax1.bar(list(map(lambda x: str(x + (width*1)), bar_resolutions_fl)), speedups_6000_1d_y, width, label="trial")
rects = ax1.bar(x-(width*1.5), speedups_2000_1d_y, width, label="1D in y, 2000", hatch='//')
ax1.bar_label(rects,fmt='%.1f', padding=3)
rects = ax1.bar(x-(width*0.5), speedups_6000_1d_y, width, label="1D in y, 6000")
ax1.bar_label(rects,fmt='%.1f', padding=3)

rects = ax1.bar(x+(width*0.5), speedups_2000_1d_x, width, label="1D in x, 2000", hatch='//')
ax1.bar_label(rects,fmt='%.1f', padding=3)
rects = ax1.bar(x+(width*1.5), speedups_6000_1d_x, width, label="1D in x, 6000",)
ax1.bar_label(rects,fmt='%.1f', padding=3)

rects = ax1.bar(x[-1] + 2.5 * width, speedups_2000_2d[0], width, label="2D, 2000", hatch='//')
ax1.bar_label(rects,fmt='%.1f', padding=3)
rects = ax1.bar(x[-1] + 3.5 * width, speedups_6000_2d[0], width, label="2D, 6000")
ax1.bar_label(rects,fmt='%.1f', padding=3)


ax1.set_xlabel("Number of cores", fontsize=13)
ax1.set_ylabel("Speedup", fontsize=13)
# ax1.set_title("Runtime of tuned code \ncompared to baseline")
ax1.set_xticks(x + width, bar_resolutions)
ax1.legend(loc='center left', bbox_to_anchor=(1, 0.5))
plt.tight_layout(rect=[0, 0, 1, 0.9])
ax1.set_title("Speedup of hybrid code", fontsize=15)
plt.show()





# for attribute, measurement in speedups_data.items():
#     offset = width * multiplier
#     rects = ax1.bar(x + offset, measurement, width, label=attribute)
#     ax1.bar_label(rects, padding=3)
#     multiplier += 1


# ax1.set_xlabel("Number of cores", fontsize=13)
# ax1.set_ylabel("Speedup", fontsize=13)
# # ax1.set_title("Runtime of tuned code \ncompared to baseline")
# ax1.set_xticks(x + width, bar_resolutions)
# ax1.legend(loc='best')

# plt.show()





# # Sequential runtimes
# runtime_seq2000 = 0.251787
# runtime_seq6000 = 2.505904

# speedups = {}
# runtimes = {}
# ncores = {}



# # Directory containing the .txt files
# dir_path = './6/hybrid/measurements'

# # Loop over all files in the directory
# for filename in os.listdir(dir_path):
#     # Check if the file ends with ".out"
#     if filename.endswith('.out'):
#         # Full path to the file
#         filepath = os.path.join(dir_path, filename)

#         # Open the file
#         with open(filepath, 'r') as file:
#             # Read the file and split it into lines
#             temp2000 = []
#             temp6000 = []
#             for i, line in enumerate(file):
#                 # Check if the line number is one of the ones you're interested in
#                 if i in [9, 32, 55, 78, 101]:
#                     # Split the line on the colon and take the second part, which is the runtime
#                     runtime2000 = float(line.split(':')[1].strip())
#                     # Add the runtime to the dictionary
#                     temp2000.append(runtime2000)
#                 if i in [18, 41, 64, 87, 110]:
#                     # Split the line on the colon and take the second part, which is the runtime
#                     runtime6000 = float(line.split(':')[1].strip())
#                     # Add the runtime to the dictionary
#                     temp6000.append(runtime6000)
            
#             average_runtime2000 = sum(temp2000) / len(temp2000)

#             # Extract the runtimes and calculate the speedups
#             key2000 = filename.split('.')[0] + ':2000'
#             runtimes[key2000] = average_runtime2000
#             speedups[key2000] = runtime_seq2000 / runtimes[key2000]
#             ncores[key2000] = int(filename.split('=')[1].split('_')[0]) * 48
            
#             average_runtime6000 = sum(temp6000) / len(temp6000)

#             # Extract the runtimes and calculate the speedups
#             key6000 = filename.split('.')[0] + ':6000'
#             runtimes[key6000] = average_runtime6000
#             speedups[key6000] = runtime_seq6000 / runtimes[key6000]
#             ncores[key6000] = int(filename.split('=')[1].split('_')[0]) * 48

# Print the speedups and runtimes
# for key in speedups:
#     print(key, speedups[key], runtimes[key], ncores[key])


# fig = plt.figure()
# ax1 = fig.add_subplot(111)

# # Get unique ncores values and sort them
# unique_ncores = sorted(set(ncores.values()))

# # Create a list of keys sorted by ncores and then by key
# keys_sorted = sorted(speedups.keys(), key=lambda x: (ncores[x], x))

# # Initialize the dictionary with an empty list
# speedups_data = {'1D in y, 2000': [], '1D in y, 6000': [], '1D in x, 2000': [], '1D in x, 6000': [], '2D, 2000': [], '2D, 6000': []}

# # Loop over the sorted keys
# for key in keys_sorted:
#     # Split the key on 'x' and check if the second part starts with a non-1 digit
#     if key.split('x')[0] == '1' and key.split('x')[1][0] != '1' :
#         if key.split(':')[1] == '2000':
#             # If it does, add the speedup to the list
#             speedups_data['1D in y, 2000'].append(np.round(speedups[key], 2))
#         if key.split(':')[1] == '6000':
#             # If it does, add the speedup to the list
#             speedups_data['1D in y, 6000'].append(np.round(speedups[key], 2))
#     elif key.split('x')[0] != '1' and key.split('x')[1][0] == '1':
#         if key.split(':')[1] == '2000':
#             # If it does, add the speedup to the list
#             speedups_data['1D in x, 2000'].append(np.round(speedups[key], 2))
#         if key.split(':')[1] == '6000':
#             # If it does, add the speedup to the list
#             speedups_data['1D in x, 6000'].append(np.round(speedups[key], 2))
#     elif key.split('x')[0] != '1' and key.split('x')[1][0] != '1':
#         if key.split(':')[1] == '2000':
#             # If it does, add the speedup to the list
#             speedups_data['2D, 2000'].append(np.round(speedups[key], 2))
#         if key.split(':')[1] == '6000':
#             # If it does, add the speedup to the list
#             speedups_data['2D, 6000'].append(np.round(speedups[key], 2))

# bar_resolutions = ['96', '144', '192'] 

# print(speedups)
# print(speedups_data)

# x = np.arange(len(bar_resolutions))  # the label locations
# width = 0.1  # the width of the bars
# multiplier = 0


# for attribute, measurement in speedups_data.items():
#     offset = width * multiplier
#     rects = ax1.bar(x + offset, measurement, width, label=attribute)
#     ax1.bar_label(rects, padding=3)
#     multiplier += 1

# ax1.set_xlabel("Number of cores", fontsize=13)
# ax1.set_ylabel("Speedup", fontsize=13)
# # ax1.set_title("Runtime of tuned code \ncompared to baseline")
# ax1.set_xticks(x + width, bar_resolutions)
# ax1.legend(loc='best')

# plt.show()


