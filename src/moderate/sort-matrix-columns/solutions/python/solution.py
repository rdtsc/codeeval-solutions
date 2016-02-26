#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    rows = (list(map(int, row.split())) for row in line.split('|'))
    rows = (list(row) for row in zip(*sorted(zip(*rows))))
    print(' | '.join(' '.join(str(n) for n in row) for row in rows))
