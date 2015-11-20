#!/usr/bin/env python3

import sys

def getCombinationCount(value, index, denominations):
  if value <= 0 or not index:
    return int(not value)

  get = lambda value, index: getCombinationCount(value, index, denominations)

  lhs = get(value, index - 1)
  rhs = get(value - denominations[index - 1], index)

  return lhs + rhs

memo = {}

for n in (int(n) for n in sys.stdin):
  if n not in memo:
    memo[n] = getCombinationCount(n, 5, [1, 5, 10, 25, 50])

  print(memo[n])
