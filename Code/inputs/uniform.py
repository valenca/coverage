from sys import argv
from random import random,seed

N=int(argv[2])
K=10

seed((argv[1],argv[2]))

#print N
#print K

for i in range(N):
	print i,random()*20,random()*15
	#print "\\fill[gray] (%3f,%3f) circle (3pt);" % (random()*20,random()*15)
