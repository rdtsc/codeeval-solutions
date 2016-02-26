#!/usr/bin/env python3

import sys

def getDecodingCount(string, index = 0):
  if len(string) - index <= 1:
    return 1

  i, j = string[index], string[index + 1]

  if (i == '2' and j >= '7') or i >= '3':
    if i == '2' and j >= '7':
      index += 1

    return getDecodingCount(string, index + 1)

  lhs = getDecodingCount(string, index + 1)
  rhs = getDecodingCount(string, index + 2)

  return lhs + rhs

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(getDecodingCount(line))
