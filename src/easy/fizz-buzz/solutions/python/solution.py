#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (x, y, n) = (int(value) for value in line.split(' '))

  translate = lambda i: ('F'*(not i % x) + 'B'*(not i % y)) or str(i)

  print(' '.join(translate(i) for i in range(1, n + 1)))
