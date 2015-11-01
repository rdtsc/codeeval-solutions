#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (n, x, y) = map(int, line.split(','))

  n = bin(n)

  print('true' if n[-x] == n[-y] else 'false')
