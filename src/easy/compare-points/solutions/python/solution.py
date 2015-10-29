#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (x0, y0, x1, y1) = map(int, line.split())

  bearing = []

  if(y0 != y1): bearing.append('S' if y0 > y1 else 'N')
  if(x0 != x1): bearing.append('W' if x0 > x1 else 'E')

  print(''.join(bearing) if bearing else 'here')
