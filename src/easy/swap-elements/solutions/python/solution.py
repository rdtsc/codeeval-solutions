#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (values, indices) = line.split(':')

  values = values.split()

  for pair in indices.split(','):
    (x, y) = map(int, pair.split('-'))
    values[x], values[y] = values[y], values[x]

  print(' '.join(values))
