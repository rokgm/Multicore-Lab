import matplotlib.pyplot as plt
import numpy as np

files = ["O0", "O1", 
          "Ofast xCORE-AVX512", "Ofast xhost", "Ofast fnoalias","all + qopt-zmm-usage=high"]
#Ofast_-fno-alias_-xhost_-qopt-zmm-usage=high
fig, (ax1, ax2) = plt.subplots(nrows=2, ncols=1, figsize=(8, 5))

for file in files:
    data = np.loadtxt(f"run_data/{file}.txt", delimiter=";", skiprows=11)
    ax1.plot(data[:,0], data[:,2], '.-', label= file)
ax1.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
ax1.set_xlabel("Resolution")
ax1.set_ylabel("MFlops")
ax1.set_title("Login Node Compiler Flag Comparison")

def draw_plot(data, offset,edge_color, fill_color):
    pos = np.arange(data.shape[1])+offset
    bp = ax2.boxplot(data, positions= pos, widths=0.3, patch_artist=True)
    for element in ['boxes', 'whiskers', 'fliers', 'medians', 'caps']:
        plt.setp(bp[element], color=edge_color)
    for patch in bp['boxes']:
        patch.set(facecolor=fill_color)

batch_run = np.loadtxt("run_data/batchrun_Ofast_zmm_xhost.txt", delimiter=';')

res500 = np.array(batch_run[0::5,2])
res1500 = np.array(batch_run[1::5,2])
res2500 = np.array(batch_run[2::5,2])
res3500= np.array(batch_run[3::5,2])
res4500= np.array(batch_run[4::5,2])
print(res500, res4500)
array = np.array([res500, res1500, res2500, res3500, res4500])
print(array)

login_run = np.loadtxt("run_data\Ofast_-fno-alias_-xhost_-qopt-zmm-usage=high_many.txt", delimiter=';')

res500 = np.array(login_run[0::5,2])
res1500 = np.array(login_run[1::5,2])
res2500 = np.array(login_run[2::5,2])
res3500= np.array(login_run[3::5,2])
res4500= np.array(login_run[4::5,2])
array2 = np.array([res500, res1500, res2500, res3500, res4500])

draw_plot(array.T, -0.2, "tomato", "white")
draw_plot(array2.T, 0.2, "skyblue", "white")
plt.xticks(range(5), [500,1500,2500,3500,4500])
plt.xlabel("Resolution")
plt.ylabel("MFlops")

plt.tight_layout()
plt.show()
