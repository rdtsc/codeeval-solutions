#!/usr/bin/env python3

import sys

mapping = \
{
  'ze': 0, 'on': 1, 'tw': 2, 'th': 3, 'fo': 4,
  'fi': 5, 'si': 6, 'se': 7, 'ei': 8, 'ni': 9
}

for line in (line.rstrip() for line in sys.stdin):
  for literal in (literal[:2] for literal in line.split(';')):
    print(mapping[literal], end='')
  print()
