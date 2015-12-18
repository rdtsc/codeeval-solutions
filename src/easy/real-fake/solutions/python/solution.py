#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  line = [int(n) for n in line.replace(' ', '')]

  tally = sum(n * 2 for n in line[0::2]) + sum(n for n in line[1::2])

  print('Fake' if tally % 10 else 'Real')
