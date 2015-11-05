#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  digits = [int(n) for n in line.replace(' ', '')]

  checksum = 0

  for i, digit in enumerate(reversed(digits), 1):
    if i & 1:
      checksum += digit

    else:
      digit *= 2
      checksum += digit - 9 if digit >= 10 else digit

  print(int(not checksum % 10))
