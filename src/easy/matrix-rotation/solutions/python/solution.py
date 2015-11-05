#!/usr/bin/env python3

import sys
import math

for line in (line.rstrip() for line in sys.stdin):
  source = line.split()
  rotated = []

  elementCount = len(source)
  width = int(math.sqrt(elementCount))

  for i in range(width):
    for j in range(1, width + 1):
      print(source[elementCount + i - width * j], end=' ')

  print()
