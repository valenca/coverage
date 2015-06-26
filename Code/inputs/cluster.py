from sys import argv
from random import random,randint,seed
from math import pi,sin,cos
from matplotlib import pyplot as plt

N=int(argv[2])
seed((argv[1],argv[2]))
	
K=randint(10,20)
v=[]
w=[]

for j in range(K):
	xx=random()*100
	yy=random()*100
	for i in range((j*N/K),(j+1)*N/K):
		d=randint(10,20)
		x=y=-1
		while(not(0<x<100 and 0<y<100)):
			t=random()*pi*2
			r=random()*d
			x=xx+(cos(t)*r)
			y=yy+(sin(t)*r)
		v.append(x)
		w.append(y)
		print i,x,y

#plt.plot(v,w,"o")
#plt.show()
	
