#!/usr/bin/env python
"""      turtle-example-suite:

        tdemo_fractalCurves.py

This program draws two fractal-curve-designs:
(1) A hilbert curve (in a box)
(2) A combination of Koch-curves.

The CurvesTurtle class and the fractal-curve-
methods are taken from the PythonCard example
scripts for turtle-graphics.
"""
from turtle import *
from time import sleep, clock

class CurvesTurtle(Pen):
    # example derived from
    # Turtle Geometry: The Computer as a Medium for Exploring Mathematics
    # by Harold Abelson and Andrea diSessa
    # p. 96-98
    def hilbert(self, size, level, parity):
        if level == 0:
            return
        # rotate and draw first subcurve with opposite parity to big curve
        self.left(parity * 90)
        self.hilbert(size, level - 1, -parity)
        # interface to and draw second subcurve with same parity as big curve
        self.forward(size)
        print (16-round(self.xcor(),2),16-round(self.ycor(),2)), "\t--",
        self.right(parity * 90)
        self.hilbert(size, level - 1, parity)
        # third subcurve
        self.forward(size)
        print (16-round(self.xcor(),2),16-round(self.ycor(),2)), "\t--",
        self.hilbert(size, level - 1, parity)
        # fourth subcurve
        self.right(parity * 90)
        self.forward(size)
        print (16-round(self.xcor(),2),16-round(self.ycor(),2)), "\t--",
        self.hilbert(size, level - 1, -parity)
        # a final turn is needed to make the turtle
        # end up facing outward from the large square
        self.left(parity * 90)

    # Visual Modeling with Logo: A Structural Approach to Seeing
    # by James Clayson
    # Koch curve, after Helge von Koch who introduced this geometric figure in 1904
    # p. 146

def main():
    ft = CurvesTurtle()

    ft.reset()
    ft.speed(0)
    ft.ht()
    ft.tracer(1,0)
    ft.pu()

    size = 0.5
    ft.setpos(-0.25,0.25)
    ft.pd()

    ta=clock()
    ft.fillcolor("red")
    ft.fill(True)
    ft.fd(size)

    ft.hilbert(size, 5, 1)

    tb=clock()
    res =  "Hilbert: %.2fsec. " % (tb-ta)
    return res

if __name__  == '__main__':
    msg = main()
    print msg
    mainloop()
