import numpy as np
from pprint import pprint
from matplotlib import pyplot as plt

labels=[]
for directory in ["results_unif10","results_clus10","results_unif20","results_clus20"]:
	w=[]
	x=[]
	with open("res/"+directory) as f:
		for j in range(30):
			v=[]
			for i in range(30):
				v.append(float(f.readline().split()[3]))
			f.readline()
			w.append(v)
	arr=np.array(w)
	labl, = plt.plot(range(1000,31000,1000),np.mean(arr,axis=1),label=directory)
	labels.append(labl)
	
plt.legend(labels)
plt.show()
