#!/usr/bin/env python3

import sys

from math import floor, sqrt

with open(sys.argv[1]) as file:
  next(file)

  for n in(int(n) for n in file):
    result = 0

    for i in range(floor(sqrt(n)) + 1):
      value = n - i * i
      test  = floor(sqrt(value))

      if test < i:
        break

      if test * test == value:
        result += 1

    print(result)
