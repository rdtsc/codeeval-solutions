#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split(',')

  print(int(lhs.endswith(rhs)))
