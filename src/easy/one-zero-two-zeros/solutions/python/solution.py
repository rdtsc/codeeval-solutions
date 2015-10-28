#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = (int(value) for value in line.split(' '))

  target = lambda i: (bin(i).count('0') - 1) == lhs

  print(sum(True for i in range(1, rhs + 1) if target(i)))
