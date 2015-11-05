#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  values = [int(n) for n in line.split()]
  friendCount = values.pop(0)

  values.sort()

  midpoint = friendCount // 2
  ourAddress = values[midpoint]

  if not friendCount & 1:
    ourAddress = (values[midpoint - 1] + values[midpoint]) // 2

  print(sum(abs(value - ourAddress) for value in values))
