from sys import argv
from random import random,seed

N=int(argv[2])
K=10

#seed((argv[1],argv[2]))

#print N
#print K

for i in range(N):
	print i,random()*100,random()*100
