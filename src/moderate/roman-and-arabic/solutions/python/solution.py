#!/usr/bin/env python3

import sys

def toDecimal(literal):
  romanBases = \
  {
    'I': 1,  'V': 5,   'X': 10,
    'L': 50, 'C': 100, 'D': 500,
    'M': 1000
  }

  result = 0
  previousBase = 1
  i = len(literal) - 1

  while i > 0:
    r  = romanBases[literal[i]]
    a  = ord(literal[i - 1]) - ord('0')
    ra = r * a
    i -= 2

    result = result - ra if previousBase > r else result + ra
    previousBase = r

  return result

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(toDecimal(line))
