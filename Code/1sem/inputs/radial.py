from math import sin,cos,pi
from matplotlib import pyplot as plt
N=50
K=10

print N
print K

x=[0]
y=[0]

print 0,0

for i in range(N-1):
	print sin(i*2*pi/N-1), cos(i*2*pi/N-1)
