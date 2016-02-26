#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (n, m) = map(int, line.split())
    locks = [False] * n

    for i in range(1, m):
      for j in range(1, n, 2):
        locks[j] = True
      for j in range(2, n, 3):
        locks[j] = not locks[j]

    locks[n - 1] = not locks[n - 1]

    print(locks.count(False))
