import numpy as np
from math import sqrt
from scipy.spatial import Delaunay
from itertools import combinations
from pprint import pprint

v=[( 2, 7),( 5, 3),( 6, 9),(10, 1),(11, 8),(13, 5),(13,10),(15, 7),(16, 3)]

w=[(8.230,0.953),(12.116,3.766),(3.479,7.617),(12.650,7.468),(2.755,0.344),(13.155,4.245),(2.966,1.124),(9.767,5.172),(0.669,4.055),(1.921,9.424),(4.922,7.166),(10.086,9.634),(8.484,5.876),(16.105,0.037),(16.754,1.447),(15.721,2.712),(1.511,3.899),(2.904,9.285),(13.618,5.788),(14.108,2.264),(1.031,2.339),(3.492,4.318),(9.477,9.995),(12.268,6.011),(6.400,4.919),(9.148,3.059),(14.815,5.528),(7.845,2.619),(8.964,7.788)]

x=[( 8, 4)]

v=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),v)
w=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),w)
x=map(lambda y: (y[0]*15/17.0,0.5+y[1]*12/13.0),x)

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

print "\\begin{figure}[H]\n\\centering\n\\begin{minipage}{0.45\linewidth}\n\\centering\n\\begin{tikzpicture}[scale=0.3]\n"

print "\\draw [<->,thick] (0,0) rectangle (15,10) {};"

tog=0
draw_tri(tri)
for j in w:
	d=300
	for i in v:
		if(dist(j,i)<d):
			if i not in x:
				d=dist(j,i)
				k=i
				tog = 0
			else:
				tog = 1
	if tog == 0:
		print("\draw [-]"+str(j)+" -- "+str(k)+";")
	if tog == 1:
		print("\draw [-,dashed]"+str(j)+" -- "+str(k)+";")

for j in w+x:
	d=300
	for i in v:
		if(dist(j,i)<d):
			d=dist(j,i)
			k=i
	if j in x:
		print("\draw [-]"+str(j)+" -- "+str(k)+";")

for i in v:
	print "\\fill [white]"+str(i)+"circle(4pt);"
	print "\\draw "+str(i)+"circle(4pt);"

for i in x:
	print "\\fill [red]"+str(i)+"circle(4pt);"
	#print "\\draw [red,thick]"+str(i)+"circle(4pt);"
	
for i in w:
	print "\\fill"+str(i)+"circle(4pt);"

print "\\end{tikzpicture}\\end{minipage}\\begin{minipage}{0.45\linewidth}\\centering\\begin{tikzpicture}[scale=0.3]"

print "\\draw [<->,thick] (0,0) rectangle (15,10) {};"

tog=0

draw_tri(tri)
for j in w:
	d=300
	for i in v:
		if(dist(j,i)<d):
			if i not in x:
				d=dist(j,i)
				k=i
				tog = 0
			else:
				tog = 1
	if tog == 0:
		print("\draw [-]"+str(j)+" -- "+str(k)+";")
	if tog == 1:
		print("\draw [-,dashed]"+str(j)+" -- "+str(k)+";")
		
for j in w+x:
	d=300
	for i in v:
		if(dist(j,i)<d):
			d=dist(j,i)
			k=i
	if j in x:
		print("\draw [-,dashed]"+str(j)+" -- "+str(k)+";")
		
for i in v:
	print "\\fill [white]"+str(i)+"circle(4pt);"
	print "\\draw "+str(i)+"circle(4pt);"

for i in x:
	print "\\fill [red]"+str(i)+"circle(4pt);"
	#print "\\draw [red,thick]"+str(i)+"circle(4pt);"
	
for i in w:
	print "\\fill"+str(i)+"circle(4pt);"
	
print "\\end{tikzpicture}\\end{minipage}\\end{figure}"
