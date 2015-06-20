from random import random,seed
from math import sqrt

seed(5)

v=sorted([(round(5.5*random()+0.25,3),round(3.5*random()+0.25,3)) for i in range(50)])

def euclidean(a,b):
	return sqrt(((a[0]-b[0])**2)+((a[1]-b[1])**2))

print "\\begin{tikzpicture}[scale=1.4]"
print "\\fill[lightgray!25,draw=black] (0,0) rectangle (6,4);"

w=[0]*len(v)

for i in range(len(v)):
    for j in range(i+1,len(v)):
        if(euclidean(v[i],v[j])<0.9):
			w[i]+=1
			w[j]+=1

for i in range(len(v)):
	if w[i]==max(w):
		x=i
		break

for i in range(len(v)):
	if euclidean(v[i],v[x])<0.9:
		w[i]=-w[i]
	else:
		w[i]=0

		
for i in range(len(v)):
    for j in range(i+1,len(v)):
        if(euclidean(v[i],v[j])<0.9):
			if w[i]>=0 and w[j]>=0:
				w[i]+=1
				w[j]+=1
				print "\\draw [thick](%g,%g) -- (%g,%g);" % (v[i][0],v[i][1],v[j][0],v[j][1])
				
for i in range(len(v)):
	if w[i]>=0:
		print "\\fill [white,draw=black] (%g,%g) circle (2.5pt);" % (v[i][0],v[i][1])
		
for i in range(len(v)):
	if w[i]==max(w):
		print "\\node [red] at (%g,%g) {\\tiny $%s$};" % (v[i][0],v[i][1],w[i])
	else:
		if w[i]>=0:
			print "\\node [blue] at (%g,%g) {\\tiny $%s$};" % (v[i][0],v[i][1],w[i])

for i in range(len(v)):
	if w[i]==max(w):
		print "\\fill [white,draw=black] (%g,%g) circle (2.5pt);" % (v[i][0],v[i][1])
		print "\\node [red] at (%g,%g) {\\tiny $%s$};" % (v[i][0],v[i][1],w[i])

for i in range(len(v)):
	if w[i]==min(w):
		print "\\fill [green,draw=black] (%g,%g) circle (2.5pt);" % (v[i][0],v[i][1])

print "\\end{tikzpicture}"	
