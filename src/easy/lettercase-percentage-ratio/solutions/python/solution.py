#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  lineLength = len(line)

  lowerCase = sum(c.islower() for c in line)
  upperCase = lineLength - lowerCase

  lowerCase /= lineLength * 0.01
  upperCase /= lineLength * 0.01

  print('lowercase: {0:.2f} uppercase: {1:.2f}'.format(lowerCase, upperCase))
