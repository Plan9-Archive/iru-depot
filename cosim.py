#!/usr/bin/env python
# Cosine similarity between two strings

from math import cos, sqrt
import sys

def cosim(s0, s1):
	d = 0
	mod = [0,0]
	for i in range(max(len(s0),len(s1))):
		if i >= len(s0): d0 = 0
		else:            d0 = ord(s0[i])
		if i >= len(s1): d1 = 0
		else:            d1 = ord(s1[i])
			
		d = d + d0*d1
		mod[0] = mod[0] + d0**2
		mod[1] = mod[1] + d1**2

	return d/(sqrt(mod[0])*sqrt(mod[1]))


if len(sys.argv) < 3:
	print "usage: " + sys.argv[0] + " s0 s1"
	sys.exit(1)

print cosim(sys.argv[1], sys.argv[2])
