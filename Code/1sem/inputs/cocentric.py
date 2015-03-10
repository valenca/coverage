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

for i in [5,10,20,30,40,60]:
	n=randint(8,12)
	for j in frange(pi/2.0/n,pi/2.0,pi/2.0/n):
		v[0].append(i*cos(j+0.1*random()))
		v[1].append(i*sin(j+0.1*random()))

N=len(v[0])
	
print(N)
print(5)

for i,j in zip(v[0],v[1]):
	print(str(abs(i))+" "+str(abs(j)))
		
