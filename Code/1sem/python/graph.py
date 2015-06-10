import numpy as np
from sys import argv
from pprint import pprint
from matplotlib import pyplot as plt

d={}
plt.style.use(plt.style.available[1])

leg=["ILP","Naive BB","Geo BB"]
#lab=["b-o","g-s","r-^"]
siz=[7,6,8]

s=40
#s=int(argv[1][-2:])
q=(argv[1][-1:])
for k,algo in enumerate(["ILP","C","cpp"]):
	print algo
	dir="../"+algo+"/outputs/"+argv[1]
	tmp=dict([(i,0) for i in range(2,s+1,2)])
	n=0.0
	with open(dir,"r") as f:
		for l in f.readlines():
			if(l[0]=='S'):
				n+=1
				continue
			if(l[0]=='P'):
				continue
			w=l.split("\t")
			if argv[1][6]=='k':
				tmp[int(w[0][4:-1])]+=float(w[3][4:-1])
			if argv[1][6]=='n':
				tmp[int(w[1][4:-1])]+=float(w[3][4:-1])
	d[algo]=np.array(sorted(list(zip(tmp.keys(),tmp.values()))))

	d[algo][:,1]/=n
	
	plt.plot(d[algo][:,0],d[algo][:,1],linewidth=2,markersize=siz[k])

	
plt.xlabel('$N$',fontsize=24)
plt.ylabel('$time(s)$',fontsize=24)
plt.yscale('log')
plt.grid(False)
plt.legend(leg,loc=4)
plt.axis([0, s+2, 10**-5, 10**4])
plt.savefig("/home/valenca/Uni/Docs/thesis/Pictures/k"+str(q)+".png", bbox_inches="tight") 
plt.show()
