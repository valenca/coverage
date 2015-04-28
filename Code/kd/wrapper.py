#!/usr/bin/env python2

from os import system as call
from sys import argv
import gtk
import math

call("./main < "+argv[1]+" > .tmp")

H=360
W=640
max_x=0.0
max_y=0.0
min_x=float("inf")
min_y=float("inf")


with open(".tmp") as f:
	N=int(f.readline())
	v=[]
	for i in range(N):
		v.append(map(float,f.readline().split()))
		max_x=max(max_x,v[-1][0])
		max_y=max(max_y,v[-1][1])
		min_x=min(min_x,v[-1][0])
		min_y=min(min_y,v[-1][1])
		
	K=int(f.readline())
	b=[]
	for i in range(K):
		b.append(map(float,f.readline().split()))
	t=float(f.readline())
	dat=f.readline()

call("cat .tmp | tail -1")
call("rm .tmp")
	
max_x=math.ceil(max_x)
max_y=math.ceil(max_y)
min_x=math.floor(min_x)
min_y=math.floor(min_y)

sc=float(max(max_x-min_x,max_y-min_y))/90.0

for i in range(len(v)):
	v[i][0]-=min_x
	v[i][1]-=min_y
	v[i][0]=(v[i][0]/sc)
	v[i][1]=(v[i][1]/sc)

for i in range(len(b)):
	b[i][0]-=min_x
	b[i][1]-=min_y
	b[i][0]=(b[i][0]/sc)
	b[i][1]=(b[i][1]/sc)

t=t/sc

max_x-=min_x
min_x=0

max_y-=min_y
min_y=0

max_x=max_x/sc
max_y=max_y/sc
min_x=min_x/sc
min_y=min_y/sc

class PyApp(gtk.Window):
	def __init__(self):
		super(PyApp, self).__init__()

		self.set_title("figure")
		self.resize(100, 100)
		self.set_position(gtk.WIN_POS_CENTER)

		self.connect("destroy", gtk.main_quit)

		darea = gtk.DrawingArea()
		darea.connect("expose-event", self.expose)
		self.add(darea)

		self.show_all()
	
	def expose(self, widget, event):
		cr = widget.window.cairo_create()
			
		w = max_x #self.allocation.width
		h = max_y #self.allocation.height

		cr.set_source_rgb(1,1,1)
		cr.rectangle(0,0,self.allocation.width,self.allocation.height)
		cr.fill()
		
		cr.translate(self.allocation.width/2.0, self.allocation.height/2.0)
		cr.scale(6,6)
		cr.translate(-max_x/2.0, max_y/2.0)
		cr.rotate(-math.pi/2)

		cr.set_line_width(0.3)
		cr.set_source_rgb(0,0,0)


		for i in v:
			cr.arc(i[0], i[1], 0.1, 0, 2*math.pi)
			cr.stroke()
		cr.set_source_rgb(1,1,1)
		cr.set_line_width(0.1)
		for i in b:
			cr.arc(i[0], i[1], t, 0, 2*math.pi)
			cr.stroke()
		cr.set_line_width(0.7)
		for i,p in enumerate(b):
			cr.set_source_rgb(1,1-(float(i)/K),0)
			#cr.set_source_rgb(0,0,0)
			cr.arc(p[0], p[1], 0.5, 0, 2*math.pi)
			cr.stroke()
			
PyApp()
gtk.main()
