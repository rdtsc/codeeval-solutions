#!/usr/bin/env python3

import sys
import math

def isPrime(n):
  if n <= 1:      return False
  elif n < 4:     return True
  elif not n & 1: return False
  elif n < 9:     return True
  elif not n % 3: return False

  limit = int(math.sqrt(n))

  i = 5

  while i <= limit:
    if not n % i or not n % (i + 2):
      return False
    i += 6

  return True

for line in (line.rstrip() for line in sys.stdin):
  (n, m) = (int(value) for value in line.split(','))

  print(sum(isPrime(i) for i in range(n, m + 1)))
