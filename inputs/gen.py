from math import sin,cos,pi
from random import randint,random
from pprint import pprint
from matplotlib import pyplot as plt
from scipy.spatial import Voronoi,voronoi_plot_2d
from scipy.spatial import Delaunay
from random import random
import numpy as np

def frange(x, y, jump):
	while x < y:
		yield x
		x += jump

v=[[],[]]
N=0

for i in [0.5,1,2,3,4,6]:
	n=randint(8,12)
	for j in frange(pi/2.0/n,pi/2.0,pi/2.0/n):
		v[0].append(i*cos(j+0.1*random()))
		v[1].append(i*sin(j+0.1*random()))
	N+=n
	
print(N)
print(5)

for i,j in zip(v[0],v[1]):
	print(str(i)+" "+str(j))
		
points=np.array(zip(*v))
	
#Voronoi
vor = Voronoi(points)
voronoi_plot_2d(vor)

#Delaunay
tri=Delaunay(points)
plt.triplot(points[:,0], points[:,1], tri.simplices.copy())
#plt.plot(points[:,0], points[:,1], 'o')

plt.show()
