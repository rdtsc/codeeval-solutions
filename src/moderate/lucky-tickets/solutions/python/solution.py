#!/usr/bin/env python3

import sys

from math import factorial as factorial
from math import floor as floor

def count(n):
  sum = 0

  nCk = lambda n, k: factorial(n) // (factorial(k) * factorial(n - k))

  for k in range(0, int(floor(9 * n / 20) + 1)):
    x = (-1) ** k
    N = n + floor(9 * n / 2) - (10 * k) - 1
    K = n - 1

    sum += x * nCk(n, k) * nCk(N, K)

  return sum

with open(sys.argv[1]) as file:
  for n in(int(n) for n in file):
    print(count(n))
