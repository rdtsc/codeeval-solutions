#!/usr/bin/env python3

import sys
import math

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    line = ''.join(c for c in line if c in '.- 0123456789').split()
    line = list(map(float, line))

    cx, cy, r, px, py = line[0], line[1], line[2], line[3], line[4]

    dx = px - cx
    dy = py - cy

    distance = math.sqrt(dx**2 + dy**2)

    print(str(distance < r).lower())
