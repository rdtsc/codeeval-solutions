#!/usr/bin/env python3

import sys

for line in (line.rstrip()[:-1] for line in sys.stdin):
  lhs = list(line)
  rhs = lhs[:]

  mapping = []
  mappingIndex = 0

  for c in sorted(lhs):
    row = rhs.index(c)

    if c == '$':
      mappingIndex = row

    mapping.append(row)
    rhs[row] = ''

  del rhs[:]

  for i in mapping:
    mappingIndex = mapping[mappingIndex]
    rhs.append(line[mappingIndex])

  print(''.join(rhs))
