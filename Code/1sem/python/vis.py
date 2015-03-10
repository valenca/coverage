from matplotlib import pyplot as plt
from scipy.spatial import Voronoi,voronoi_plot_2d,Delaunay
from random import random,seed
from itertools import combinations
from math import sqrt
from pprint import pprint
from datetime import datetime
from collections import defaultdict
import numpy as np

global c1
global c2

c1=0
c2=0

def voronoi(points):
	vor = Voronoi(points)
	voronoi_plot_2d(vor)

def delaunay(points):
	tri=Delaunay(points,incremental=True)
	plt.triplot(points[:,0], points[:,1], tri.simplices.copy())
	#plt.plot(points[:,0], points[:,1],'o',color="black")

def scatter(points,col='black'):
	plt.scatter(points[:,0],points[:,1],color=col)

def dist(t1,t2):
	return sqrt( (t1[0]-t2[0])**2 + (t1[1]-t2[1])**2)

def dist2(t1,t2):
	return sqrt( (t1[0]-t2[0])**2 + (t1[1]-t2[1])**2)

def maxdist(v,w):
	mx=0
	for i in w:
		mn=float('inf')
		for j in v:
			d=dist(i,j)
			mn=min(mn,d)
		mx=max(mx,mn)   #minmax
		#mx+=mn         #minsum
	return mx,["",""]

def maxdist2(p,v):
	print "start:\t\t",str(datetime.now().time())
	
	mesh=Delaunay(p)	
	print "delaunay:\t",str(datetime.now().time())
	mx=0
	adj=defaultdict(set)
	for k in mesh.vertices:
		for i,j in combinations(k,2):
			adj[i].add(j)
			adj[j].add(i)
			
	for i in range(len(adj)):
		adj[i].add(i)
		
	print "reverse sets:\t",str(datetime.now().time())
	
	last=0
	for i in v:
		mn,last=recursive(i,last,p,adj)
		if mn>mx:
			mx=mn
			t=p[last],i
	print "compute:\t",str(datetime.now().time())
	
	return mx,t

def recursive(i,j,p,adj):
	mn=float('inf')
	mz=j
	for point in adj[j]:
		d=dist2(i,p[point])
		if(d<mn):
			mn=d
			mz=point
	if mz==j:
		return mn,mz
	else:
		return recursive(i,mz,p,adj)

def brute(v,k):
	m=float('inf')
	pm=[]
	for p in list(combinations(v,k)):
		d=maxdist2(p,v)[0]
		if m > d:
			m=d
			pm=p
	return np.array(pm),m

if __name__ == "__main__":
	
	N = input()
	K = input()
	v=[]

	for i in xrange(N):
		v.append(map(float,raw_input().split()))
		
	pm=[]
	m = float('inf')
	for p in combinations(v,K):
		d=maxdist(p,v)
		if m > d[0]:
			m=d[0]
			pm=p
			print m
			
	pprint((pm,m))


