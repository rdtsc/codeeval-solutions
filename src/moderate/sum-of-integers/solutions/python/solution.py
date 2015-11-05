#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  terms = [int(n) for n in line.split(',')]

  maxSum = terms[0]
  localMax = maxSum

  for i in range(1, len(terms)):
    localMax = max(terms[i], localMax + terms[i])
    maxSum = max(maxSum, localMax)

  print(maxSum)
