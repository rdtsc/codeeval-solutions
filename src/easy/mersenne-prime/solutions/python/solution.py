#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for n in (int(n) for n in file):
    print(', '.join(str(p) for p in [3, 7, 31, 127, 2047] if p < n))
