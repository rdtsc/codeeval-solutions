#!/usr/bin/env python3

import sys

def getVariantCount(string):
  lhs = string[:len(string) >> 1]
  rhs = string[len(string) >> 1:]

  asteriskCount = 0

  for i in range(len(lhs)):
    a, b = lhs[i], rhs[i]

    if a == '*' and b == '*':
      asteriskCount += 1

    elif a != b and a != '*' and b != '*':
      return 0

  return 2 ** asteriskCount

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(getVariantCount(line))
