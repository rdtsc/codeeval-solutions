#!/usr/bin/env python3

import sys

isPrime = lambda n: \
[
  0, 0, 1, 1, 0, 1, 0, 1, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
  0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 1, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
  0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0
][n]

def getShortestRepetitionLength(string):
  length = len(line)

  if line[0] * length == line:
    return 1

  elif isPrime(length):
    return length

  else:
    for i in range(2, length // 2 + 1):
      for j in range(i, length, i):
        if line[:i] == line[i:2 * j]:
          return i

  return len(line)

for line in (line.rstrip() for line in sys.stdin):
  print(getShortestRepetitionLength(line))
