#!/usr/bin/env python3

import sys

def isSelfDescribing(n):
  digits = [int(i) for i in n]
  frequencies = [0] * 10

  for digit in digits:
    frequencies[digit] += 1

  for i, digit in enumerate(digits):
    if frequencies[i] != digit:
      return False

  return True

for line in (line.rstrip() for line in sys.stdin):
  print(int(isSelfDescribing(line)))
