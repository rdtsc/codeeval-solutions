#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for n in(int(n) for n in file):
    result = []

    while n > 0:
      n -= 1
      result.insert(0, ord('A') + (n % 26))
      n //= 26

    print(''.join(map(chr, result)))
