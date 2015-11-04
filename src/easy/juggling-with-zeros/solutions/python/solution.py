#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  result = 0

  for flag, sequence in zip(*[iter(line.split())] * 2):
    invert = len(flag) == 2

    for i in sequence:
      result <<= 1
      result += invert

  print(result)
