#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  numbers = ','.join(v for v in line.split(',') if v.isdigit())
  strings = ','.join(v for v in line.split(',') if not v.isdigit())

  print(strings, '|' if numbers and strings else '', numbers, sep='')
