from itertools import combinations
from pprint import pprint
from math import sqrt
from string import *

N = input()
K = input()
v=[]

for i in xrange(N):
	v.append(map(float,raw_input().split()))

def dist(a,b):
	return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

def maxdist(v,w):
	mx=0
	for i in w:	
		mn=float('inf')
		for j in v:
			d=dist(i,j)
			mn = min(mn, d)
		mx=max(mx,mn)   #minmax
		#mx+=mn         #minsum
	return mx

pm=[]
m = 1000000
print "going"
for p in list(combinations(v,K)):
	print p
	d=maxdist(p,v)
	if m > d:
		m=d
		pm=p
		print m

pprint((pm,m))

	

