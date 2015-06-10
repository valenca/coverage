import numpy as np
from pprint import pprint
from matplotlib import pyplot as plt

plt.style.use(plt.style.available[1])

labels=[]
#for directory in ["results_unif10","results_unif20","results_unif30"]:
for directory in ["results_unif10"]:
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
	plt.plot(range(1000,31000,1000),
					 np.mean(arr,axis=1),
					 linewidth=1.5)
	labels.append("distance = "+directory[-2:]+"%")
	
plt.yscale('log')
#plt.axis([0, s+2, 10**-5, 10**4])
plt.xlabel('N')
plt.ylabel('time(s)')
plt.grid(False)
plt.legend(labels,loc=4)
plt.show()

labels=[]
for directory in ["results_unif30k","results_unif20k","results_unif10k"]:
	w=[]
	x=[]
	with open("res/"+directory) as f:
		for j in range(8):
			v=[]
			for i in range(30):
				v.append(float(f.readline().split()[2]))
			f.readline()
			w.append(v)
	arr=np.array(w)
	plt.plot(range(10,50,5),np.mean(arr,axis=1),linewidth=1.5)
	labels.append("N = "+directory[-3:-1]+"000")
	
#plt.yscale('log')
#plt.axis([15, 45, 10**1, 10**2])
plt.ylabel('K')
plt.xlabel('minimum distance(%)')
plt.grid(False)
plt.legend(labels,loc=1)
plt.show()
