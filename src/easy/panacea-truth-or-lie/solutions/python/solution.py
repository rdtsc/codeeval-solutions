#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (lhs, rhs) = line.split('|')

    lhs = sum(map(lambda n: int(n, 16), lhs.split()))
    rhs = sum(map(lambda n: int(n,  2), rhs.split()))

    print(lhs <= rhs)
