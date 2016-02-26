#!/usr/bin/env python3

import sys
import collections

Point = collections.namedtuple('Point', 'x1 y1 x2 y2')

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    line = list(map(int, line.split(',')))

    lhs = Point(x1=line[0], y1=line[1], x2=line[2], y2=line[3])
    rhs = Point(x1=line[4], y1=line[5], x2=line[6], y2=line[7])

    x = lhs.x1 > rhs.x2 or lhs.x2 < rhs.x1
    y = lhs.y1 < rhs.y2 or lhs.y2 > rhs.y1

    print(not (x or y))
