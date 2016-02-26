#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (lhs, rhs) = line.split(',')
    print(''.join(c for c in lhs if c not in rhs[1:]))
