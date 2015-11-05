#!/usr/bin/env python3

import sys
import math

terms = [int(n) for n in sys.stdin]
maxTerm = max(terms)

isPrime = [True] * maxTerm

for i in range(2, int(math.sqrt(maxTerm)) + 1):
  if isPrime[i]:
    for j in range(i * i, maxTerm, i):
      isPrime[j] = False

for term in terms:
  print(','.join(str(i) for i in range(2, term) if isPrime[i]))
