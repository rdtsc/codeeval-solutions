#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  n = int(line)
  digitCount = len(line)

  print(sum(digit**digitCount for digit in map(int, line)) == n)
