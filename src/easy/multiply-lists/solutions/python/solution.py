#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split('|')

  lhs = map(int, lhs.split())
  rhs = map(int, rhs.split())

  print(*(x * y for x, y in zip(lhs, rhs)))
