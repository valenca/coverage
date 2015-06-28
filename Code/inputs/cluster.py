#!/usr/bin/env python2

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
	xx=random()*100 #random centroid position
	yy=random()*100
	for i in range((j*N/K),(j+1)*N/K): 
		d=randint(10,20)   #radius of the cluster
		x=y=-1
		while(not(0<x<100 and 0<y<100)): #make sure all points land within [(0,0) and (100,100)]
			t=random()*pi*2  #polar coordinates around centroid
			r=random()*d    
			x=xx+(cos(t)*r)
			y=yy+(sin(t)*r)
		v.append(x)
		w.append(y)
		#print "\t\\fill[white,draw=black] (%.3f,%.3f) circle (1.5pt);"%(x/100.0*6,y/100.0*4)
		print i,x,y

plt.plot(v,w,"o")
plt.show()
	
