from os import system as call
from sys import argv
from itertools import izip
from pprint import pprint
from math import sqrt
import time
#with open(argv[1]) as f:
#	v=map(int,f.read().split())

def dist(a,b):
	return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

N=input()
K=input()
v=[]

start = time.time()

for i in xrange(N):
	v.append(map(float,raw_input().split()))

x_max=0.0
x_min=float("inf")
	
for i in v:
	x_max=max(x_max,i[0])
	x_min=min(x_min,i[0])

D=(x_max-x_min)*0.2
print D
d=dict()
	
for i,vi in enumerate(v):
	for j,vj in enumerate(v):
		if(dist(vi,vj)>D):
			d[i+1,j+1]=0
		else:
			d[i+1,j+1]=1
		
with open("../ILP/distances.dat","w") as f:
	
	f.write("data;\nparam N:= %d;" % N)
	f.write("\nparam D:= %f;" % D)
	
	f.write("\nparam C: ")

	for i in xrange(1,N+1):
		d[(i,i)]=0
		f.write(str(i)+" ")
	f.write(":=\n")
	
	for i in xrange(1,N+1):
		f.write(str(i))
		for j in xrange(1,N+1):
			f.write(" "+str(d[(i,j)]))
		f.write("\n")
	f.write(";\nend;\n")

if len(argv)==2:
	call("glpsol -m ../ILP/form.mod -d ../ILP/distances.dat -y ../ILP/output --nomip > /dev/null")
else:
	call("glpsol -m ../ILP/form.mod -d ../ILP/distances.dat -y ../ILP/output --nomip")
call("cat ../ILP/output | tail -1")
end = time.time()
print ("T = %f" % float(end-start))
call("rm ../ILP/distances.dat")
