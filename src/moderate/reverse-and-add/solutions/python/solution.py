#!/usr/bin/env python3

import sys

reverse = lambda value: int(str(value)[::-1])

for n in (int(n) for n in sys.stdin):
  stepCount = 1
  m = n + reverse(n)

  while m != reverse(m):
    m += reverse(m)
    stepCount += 1

  print(stepCount, m)
