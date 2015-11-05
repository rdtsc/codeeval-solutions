#!/usr/bin/env python3

import sys

for n in (int(n) for n in sys.stdin):
  result = 0

  for denomination in [5, 3]:
    result += (n // denomination)
    n -= (n // denomination) * denomination

  print(result + n)
