#!/usr/bin/env python3

import sys

from math import factorial as f

nCk = lambda n, k: f(n) // (f(k) * f(n - k))

with open(sys.argv[1]) as file:
  for n in(int(n) for n in file):
    print(*(nCk(d, i) for d in range(0, n) for i in range(0, d + 1)))
