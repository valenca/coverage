from sys import argv
from random import random,seed
from math import sqrt
from scipy.spatial import distance

N=int(argv[2])
K=50
S=3
seed((argv[1],argv[2]))

#print N
#print K
k=0
for i in range(K):
	x=(random()*100,random()*100)
	j=1
	print k,x[0],x[1]
	k+=1
	while(j < (N/K)+1):
		y=(random()*S*2)+x[0]-S,(random()*S*2)+x[1]-S
		if(distance.euclidean(x,y)<=S):
			print k,y[0],y[1]
			j+=1
			k+=1
