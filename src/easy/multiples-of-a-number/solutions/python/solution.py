#!/usr/bin/env python3

import sys
import math

for line in (line.rstrip() for line in sys.stdin):
  (x, n) = map(int, line.split(','))

  multiple = x >> int(math.log(n, 2))

  if multiple * n != x:
    multiple += 1

  print(n * multiple)
