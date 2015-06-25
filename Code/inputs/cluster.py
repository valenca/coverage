from sys import argv
from random import random,randint,seed
from math import pi,sin,cos
from matplotlib import pyplot as plt

N=int(argv[2])
seed((argv[1],argv[2]))
	
K=randint(10,20)
d=15
v=[]
w=[]

for i in range(K):
	xx=random()*100
	yy=random()*100
	for i in range(N/K):
		x=y=-1
		while(not(0<x<100 and 0<y<100)):
			t=random()*pi*2
			r=random()*d
			x=xx+(cos(t)*r)
			y=yy+(sin(t)*r)
		v.append(x)
		w.append(y)

plt.plot(v,w,"o")	
plt.show()
