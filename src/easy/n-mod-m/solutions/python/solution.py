#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (n, m) = map(int, line.split(','))

  print(n - (n // m) * m)
