#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  rows = \
  (
    (int(cell) for cell in row.split(' ') if cell) for row in line.split('|')
  )

  print(' '.join(str(max(scores)) for scores in zip(*rows)))
