#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split()

  (h0, m0, s0) = map(int, lhs.split(':'))
  (h1, m1, s1) = map(int, rhs.split(':'))

  lhs = h0 * 3600 + m0 * 60 + s0
  rhs = h1 * 3600 + m1 * 60 + s1

  ds = abs(lhs - rhs)

  print('{:02}:{:02}:{:02}'.format(ds // 3600,
                                   ds // 60 % 60,
                                   ds % 60))
