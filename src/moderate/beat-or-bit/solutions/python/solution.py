#!/usr/bin/env python3

import sys

def decode(n):
  n ^= (n >> 1)
  n ^= (n >> 2)
  n ^= (n >> 4)
  return n

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    values = filter(None, line.replace('|', '').split())
    values = map(decode, map(lambda n: int(n, 2), values))
    print(' | '.join(map(str, values)))
