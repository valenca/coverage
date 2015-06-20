import numpy as np
from pprint import pprint
from matplotlib import pyplot as plt
from matplotlib import ticker
from itertools import product

plt.style.use(plt.style.available[1])

lista=["LS","BP10","BP5"]

trans={"KD"    :{"10":"#00CDCD","15":"#388E8E","20":"#2F4F4F"},
	   "LS"    :{"10":"#FF3030","15":"#CD0000","20":"#8D0000"},
	   "RS5"   :{"10":"#49E20E","15":"#308014","20":"#3A6629"},
	   "RS10"  :{"10":"#FFCC11","15":"#FFB90F","20":"#8B6508"},
	   "BP10"  :{"10":"#CC00FF","15":"#9A32CD","20":"#4B0082"},
	   "BP5"   :{"10":"#87421F","15":"#8A360F","20":"#691F01"},
}

labels=[]
for directory in product(lista,["results_unif10","results_unif15","results_unif20"]):
	w=[]
	x=[]
	with open(directory[0]+"/res/"+directory[1]) as f:
		for j in range(30):
			v=[]
			for i in range(30):
				v.append(float(f.readline().split()[3]))
			f.readline()
			w.append(v)
	arr=np.array(w)
	plt.plot(range(1000,31000,1000),
					 np.mean(arr,axis=1),
					 linewidth=2,color=trans[directory[0]][directory[1][-2:]])
	labels.append(directory[0]+" d="+directory[1][-2:]+"%")
	
plt.yscale('log')
#plt.axis([0, s+2, 10**-5, 10**4])
plt.xticks(range(0,31000,10000))
plt.xlabel('N')
plt.ylabel('time(s)')
plt.grid(False)
plt.subplots_adjust(bottom=0.15,right=0.65)
#plt.legend(labels,loc=4)
plt.legend(labels,loc='center left', bbox_to_anchor=(1, 0.5))
plt.show()

labels=[]
for directory in product(lista,["results_unif10","results_unif15","results_unif20"]):
	w=[]
	x=[]
	with open(directory[0]+"/res/"+directory[1]) as f:
		for j in range(30):
			v=[]
			for i in range(30):
				v.append(float(f.readline().split()[2]))
			f.readline()
			w.append(v)
	arr=np.array(w)
	plt.plot(range(1000,31000,1000),
			 np.mean(arr,axis=1),
			 linewidth=1.5,color=trans[directory[0]][directory[1][-2:]])
	labels.append(directory[0]+" d="+directory[1][-2:]+"%")
	
#plt.yscale('log')
#plt.axis([15, 45, 10**1, 10**2])
plt.ylabel('K')
plt.xlabel('N')
plt.xticks(range(0,31000,10000))
plt.grid(False)
plt.subplots_adjust(bottom=0.15,right=0.65)
plt.legend(labels,loc='center left', bbox_to_anchor=(1, 0.5))
plt.show()
