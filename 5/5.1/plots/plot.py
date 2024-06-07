import matplotlib.pyplot as plt
import numpy as np

########
# DATA #
########
across_nodes = np.array([
    [0, 0.480701],     
    [1, 1.201900],     
    [2, 2.469195],     
    [3, 4.982561],     
    [4, 8.844114],     
    [5, 16.507835],    
    [6, 34.164998],    
    [7, 66.469485],    
    [8, 129.997875],   
    [9, 252.028961],   
    [10,468.040605],   
    [11,818.874983],   
    [12,1349.781334],  
    [13,1933.105292],  
    [14,2383.138585],  
    [15,3530.908377],  
    [16,3258.098822],  
    [17,5479.454535],  
    [18,7404.784856],  
    [19,9146.248574],  
    [20,10287.980385], 
    [21,10665.993306], 
    [22,10987.863108], 
    [23,11287.934560], 
    [24,11604.398328]
])

across_sockets = np.array([
    [0, 0.344557],    
    [1, 0.840904],    
    [2, 2.392926],    
    [3, 8.544988],    
    [4, 17.189839],   
    [5, 29.784278],   
    [6, 60.227412],   
    [7, 105.029080],  
    [8, 194.736180],  
    [9, 336.373757],  
    [10,625.255404],  
    [11,1084.855234], 
    [12,1478.215711], 
    [13,2726.152008], 
    [14,3266.146954], 
    [15,3675.767405], 
    [16,4129.388496], 
    [17,4543.952837], 
    [18,5152.727925], 
    [19,5571.592855], 
    [20,5534.986866], 
    [21,5609.579636], 
    [22,5601.892010], 
    [23,5649.212957], 
    [24,5599.048758]
])

one_socket = np.array([
    [0, 0.428383],      
    [1, 1.061073],      
    [2, 3.086796],      
    [3, 11.927814],     
    [4, 23.988870],     
    [5, 41.547447],     
    [6, 87.289430],     
    [7, 155.175515],    
    [8, 307.834689],    
    [9, 434.802681],    
    [10,829.031734],    
    [11,1464.960732],   
    [12,2349.559535],   
    [13,3494.534592],   
    [14,4564.466977],   
    [15,6025.422838],   
    [16,7072.235596],   
    [17,9165.043296],   
    [18,10540.120678],  
    [19,11896.474007],  
    [20,9850.570420],   
    [21,11369.492343],  
    [22,11213.430490],  
    [23,8588.228701],   
    [24,7987.526339]
])

#########
# PLOTS #
#########

fig, ax1 = plt.subplots(nrows=1, ncols=1, figsize=(8, 5))

ax1.set_xlabel("Message Size [Byte]")
ax1.set_ylabel("Bandwidth [MByte/s]")
ax1.plot(across_nodes[:,0], across_nodes[:,1], '.-', label="across nodes")
ax1.plot(across_sockets[:,0], across_sockets[:,1], '.-', label="across sockets")
ax1.plot(one_socket[:,0], one_socket[:,1], '.-', label="one socket")
ax1.set_xticks(across_nodes[:,0])
xlables = [str(int(i)) for i in across_nodes[:,0]]
ax1.set_xticklabels([r"$2^{" + i + r"}$" for i in xlables])

ax1.set_title("Group 2:  Bandwidth for Communication")
ax1.grid(True, linestyle='--')
ax1.legend()
plt.tight_layout()
plt.savefig("group2_bandwidth.png", dpi=200)
plt.show()
plt.close()