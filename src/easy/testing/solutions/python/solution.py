#!/usr/bin/env python3

import sys

classify = lambda n: 'Done'     if n == 0 else \
                     'Low'      if n <= 2 else \
                     'Medium'   if n <= 4 else \
                     'High'     if n <= 6 else \
                     'Critical'

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split('|')

  lhs = lhs.rstrip()
  rhs = rhs.lstrip()

  mistakes = sum(lhs[i] != rhs[i] for i in range(min(len(lhs), len(rhs))))

  print(classify(mistakes))
