import numpy as np
from pprint import pprint
import matplotlib 
from matplotlib import ticker
from itertools import product

from matplotlib import pyplot as plt

plt.style.use(plt.style.available[1])

lista=["LS","KD"]#"RS5","RS10"]#,"BP5","BP10","BP20"]
#files=["results_unif10","results_clus10"]
#files=["results_unif15","results_clus15"]
files=["results_unif20","results_clus20"]

trans={"KD"		:{"10":"#00CDCD","15":"#388E8E","20":"#2F4F4F"},
	   "LS"		:{"10":"#FF3030","15":"#CD0000","20":"#8D0000"},
	   "RS5"	:{"10":"#49E20E","15":"#308014","20":"#3A6629"},
	   "RS10"	:{"10":"#FFCC11","15":"#FFB90F","20":"#8B6508"},
	   "BP10"	:{"10":"#CC00FF","15":"#9A32CD","20":"#4B0082"},
	   "BP20"	:{"10":"#87421F","15":"#8A360F","20":"#691F01"},
}

names={"KD"		: "kd-tree",
	   "LS"		: "Line Sweep",
	   "RS5"	: "Random Sample 5%",
	   "RS10"	: "Random Sample 10%",
	   "BP5"	: "Two-phase 5%",
	   "BP10"	: "Two-phase 10%",
	   "BP20"	: "Two-phase 20%",
}

trans={"KD"		: "#0000FF",
	   "LS"		: "#FF0000",
	   "RS5"	: "#0000FF",
	   "RS10"	: "#00CC00",
	   "BP5"	: "#0000FF",
	   "BP10"	: "#00CC00",
	   "BP20"	: "#FF8800",
}

labels=[]
plot_n=1

#plt.figure(figsize=(15,4))

for files in [["results_unif10","results_clus10"],["results_unif15","results_clus15"],["results_unif20","results_clus20"]]:
	plt.subplot(130+plot_n)
	plot_n+=1
	for directory in product(lista,files):
		if directory[1][-3]=='f':
			l='--'
			t='Clusters'
		else:
			l='-'
			t='Uniform'
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
				 np.mean(arr,axis=1),l,
				 linewidth=2,color=trans[directory[0]])
		labels.append(names[directory[0]]+" "+t)
	
	plt.yscale('log')
	plt.xticks(range(0,31000,10000))
	plt.axis([0, 30000, 0, 10])
	plt.xlabel('$N$')
	plt.ylabel('$time(s)$')
	plt.grid(False)
	plt.title("$ d=%d\\%% $"%int(files[0][-2:]))
	#plt.tight_layout()
	plt.subplots_adjust(bottom=0.3,left=0.07,right=0.95)

	if(plot_n==3):
		plt.legend(labels,loc=9, ncol=len(lista),bbox_to_anchor=(0.5,-0.2))

plt.show()

plt.figure(figsize=(12,4))
labels=[]
plot_n=1
for files in [["results_unif10","results_clus10"],["results_unif15","results_clus15"],["results_unif20","results_clus20"]]:
	plt.subplot(130+plot_n)
	plot_n+=1
	for directory in product(lista,files):
		if directory[1][-3]=='f':
			l='--'
			t='Clusters'
		else:
			l='-'
			t='Uniform'
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
				 np.mean(arr,axis=1),l,
				 linewidth=2,color=trans[directory[0]])
		labels.append(names[directory[0]]+" "+t)

	
	plt.axis([0, 30000, 0, 100])
	plt.xticks(range(0,31000,10000))
	plt.xlabel('$N$')
	plt.ylabel('$K$')
	plt.grid(False)
	plt.title("$ d=%d\\%% $"%int(files[0][-2:]))
	#plt.tight_layout()
	plt.subplots_adjust(bottom=0.3,left=0.07,right=0.95)
	#plt.legend(labels,loc=4)

	if(plot_n==3):
		plt.legend(labels,loc=9, ncol=len(lista),bbox_to_anchor=(0.5,-0.2))

plt.show()
