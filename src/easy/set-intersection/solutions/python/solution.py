#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split(';')

  lhs = map(int, lhs.split(','))
  rhs = map(int, rhs.split(','))

  intersection = sorted(set(lhs).intersection(rhs))

  print(','.join(map(str, intersection)) if intersection else '')
