import numpy as np
from math import sqrt
from scipy.spatial import Delaunay
from itertools import combinations
from pprint import pprint

v=[( 2, 7),( 5, 3),( 6, 9),(11, 8),(13,10),(8,4)]

vl=[(10, 1),(13, 5),(15, 7),(16, 3)]

w=[(8.230,0.953),(12.116,3.766),(3.479,7.617),(12.650,7.468),(2.755,0.344),(13.155,4.245),(2.966,1.124),(9.767,5.172),(0.669,4.055),(1.921,9.424),(4.922,7.166),(10.086,9.634),(8.484,5.876),(16.105,0.037),(16.754,1.447),(15.721,2.712),(1.511,3.899),(2.904,9.285),(13.618,5.788),(14.108,2.264),(1.031,2.339),(3.492,4.318),(9.477,9.995),(12.268,6.011),(6.400,4.919),(9.148,3.059),(14.815,5.528),(7.845,2.619),(8.964,7.788)]

x=[]
sd=[(1.031,2.339),(8.230,0.953)]

v=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),v)
vl=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),vl)
w=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),w)
x=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),x)
sd=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),sd)

points=np.array(v)
tri=Delaunay(points,incremental=True)

def draw_tri(tria):
	st=set([])
	for j in  points[tria.simplices]:
		i=map(tuple,j.tolist())
		st |= set(tuple(combinations(i,2)))

	for i in st:
		print "\draw [-,gray!50] (%.3f,%.3f) -- (%.3f,%.3f);" % (i[0][0],i[0][1],i[1][0],i[1][1])

def dist(a,b):
	return sqrt(((b[1]-a[1])**2)+((b[0]-a[0])**2))

print "\\begin{figure}[H]\n\\begin{tikzpicture}[scale=0.4]\n"

print "\\draw [<->,thick] (0,0) rectangle (15,10) {};"

print "\\draw [-,dashed]"+str(sd[0])+" -- " + str(sd[1])+";"

tog=0
#draw_tri(tri)
for j in w:
	d=300
	for i in v+vl:
		if(dist(j,i)<d):
			if i in v:
				d=dist(j,i)
				k=i
				tog = 0
			if i in vl:
				tog = 1
	if tog == 0:
		if j not in sd:
			print("\draw [-]"+str(j)+" -- "+str(k)+";")
		else:
			print("\draw [-,thick,red]"+str(j)+" -- "+str(k)+";")
		print "\\fill"+str(j)+"circle(4pt);"
	if tog == 1:
		#print("\draw [-,dashed]"+str(j)+" -- "+str(k)+";")
		print "\\fill [gray]"+str(j)+"circle(4pt);"
		#print "\\draw [gray]"+str(j)+"circle(4pt);"
		
for j in w+x:
	d=300
	for i in v:
		if(dist(j,i)<d):
			d=dist(j,i)
			k=i
	if j in x:
		print("\draw [-]"+str(j)+" -- "+str(k)+";")

for j in vl:
	print "\\fill [gray]"+str(j)+"circle(4pt);"
	#print "\\draw [gray]"+str(j)+"circle(4pt);"
	
for j in v:
	print "\\fill [white]"+str(j)+"circle(4pt);"
	print "\\draw [black]"+str(j)+"circle(4pt);"
	
	
print "\\end{tikzpicture}\\end{figure}"
