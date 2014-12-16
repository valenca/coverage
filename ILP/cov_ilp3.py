from os import system as call
from sys import argv
from itertools import izip
from pprint import pprint
from math import sqrt

#with open(argv[1]) as f:
#	v=map(int,f.read().split())

def dist(a,b):
	return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

N=input()
K=input()
v=[]

for i in xrange(N):
	v.append(map(float,raw_input().split()))

d=dict()
s=[]

for i,vi in enumerate(v):
	for j,vj in enumerate(v):
		d[i+1,j+1]=dist(vi,vj)
		if(i>j):
			s.append(dist(vi,vj))

s=sorted(list(set(s)))

with open("distances.dat","w") as f:

	f.write("data;\nparam N:= %d;" % N)
	f.write("\nparam K:= %d;" % K)
	f.write("\nparam L:= %d;" % len(s))
	
	
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
	f.write(";")
	
	f.write("\nparam D:=\n")
	for i in xrange(1,len(s)+1):
		f.write("\t"+str(i)+"\t"+str(s[i-1])+"\n")
	f.write(";")
	
	f.write("\nend;\n")

call("glpsol -m form3.mod -d distances.dat -y output")
call("cat output")
call("rm distances.dat")
