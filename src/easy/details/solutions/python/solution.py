#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  print(0 if 'XY' in line else min(row.count('.') for row in line.split(',')))
