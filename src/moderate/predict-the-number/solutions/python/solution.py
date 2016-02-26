#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for n in(int(n) for n in file):
    print(bin(n).count('1') % 3)
