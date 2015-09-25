from sys import argv
from random import random,seed

N=int(argv[2])
K=10

seed((argv[1],argv[2]))

#print K

print 0
print 0
print 20
print 15
print 0.1
print N
print 5

for i in range(N):
	print random()*20,random()*15
	#print "\\fill[gray] (%3f,%3f) circle (3pt);" % (random()*20,random()*15)

print 0,0
print 20,0
print 0,15
print 20,15
print -1,10
