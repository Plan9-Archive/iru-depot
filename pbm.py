#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math, sys

class Screen:
	def __init__(self, w, h):
		self.w = w
		self.h = h
		self.data = [[0] * w for i in range(0, h)]
		self.objs = []

	def add(self, obj):
		self.objs.append(obj)

	def __str__(self):
		for obj in self.objs:
			obj.plot(self)

		s = "P1\n%d %d\n" % (self.w, self.h)
		for i in range(len(self.data)-1, -1, -1):
			for j in range(0, len(self.data[i])):
				s += "%d " % self.data[i][j]
			s += "\n"
		return s

class Point:
	def __init__(self, x, y):
		self.x = x
		self.y = y
	def plot(self, scr):
		scr.data[self.x][self.y] = 1

class Circle:
	def __init__(self, x, y, r):
		self.x = x
		self.y = y
		self.r = r

	def plot(self, scr):
		t = 0
		while t < 2*math.pi:
			p = Point(self.x+int(self.r*math.cos(t)), self.y+int(self.r*math.sin(t)))
			scr.add(p)
			t += math.pi/(self.r*self.r)

if len(sys.argv) < 6:
	sys.stderr.write("usage: %s x y r w h\n" % sys.argv[0])
	sys.exit(1)

scr = Screen(int(sys.argv[4]), int(sys.argv[5]))
c = Circle(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
scr.add(c)
print scr
