from sys import argv
from random import random,seed

N=500
K=10

seed(argv[1])

print N
print K


for i in range(N):
	print random()*100,random()*100
