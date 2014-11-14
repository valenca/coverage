from itertools import combinations
from pprint import pprint
from math import sqrt

v=[(2,3),
	(12,2),
	(7,7),
	(1,4),
	(4,4),
	(3,5),
	(3.5,1),
	(6,5),
	(9,8),
	(6,8),
	(11,1),
	(12,5),
	(13,3)]

#pprint(list(combinations(v,3)))

def dist(t1,t2):
	return sqrt( (t1[0]-t2[0])**2 + (t1[1]-t2[1])**2)

def maxdist(v,w):
	mx=0
	for i in w:
		mn=30
		for j in v:
			d=dist(i,j)
			mn=min(mn,d)
		mx=max(mx,mn)   #minmax
		#mx+=mn         #minsum
	return mx

if __name__=='__main__':
	m=30
	pm=[]
	for p in list(combinations(v,3)):
		d=maxdist(p,v)
		if m > d:
			m=d
			pm=p

	print(pm,m)

	

