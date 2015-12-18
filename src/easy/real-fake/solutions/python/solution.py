#!/usr/bin/env python3

import sys

for line in sys.stdin:
  digits = [int(c) for c in line if c.isdigit()]

  tally = sum(n * 2 for n in digits[::2]) + sum(digits[1::2])

  print('Fake' if tally % 10 else 'Real')
