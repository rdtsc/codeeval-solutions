#!/usr/bin/env python3

import sys

def isHappy(n):
  reduce = lambda n: sum(d**2 for d in map(int, str(n)))

  n = reduce(n)
  chain = set()

  while n != 1:
    if n in chain:
      return False

    chain.add(n)
    n = reduce(n)

  return True

for n in (int(n) for n in sys.stdin):
  print(int(isHappy(n)))
