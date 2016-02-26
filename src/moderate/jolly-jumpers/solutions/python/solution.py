#!/usr/bin/env python3

import sys

def isJolly(terms):
  termCount = len(terms)

  if termCount == 1:
    return True

  used = [False] * termCount

  for i in range(termCount - 1):
    delta = abs(terms[i] - terms[i + 1])

    if not delta or delta >= termCount or used[delta - 1]:
      return False

    used[delta - 1] = True

  return True

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    terms = list(map(int, line.split()))[1:]
    print('Jolly' if isJolly(terms) else 'Not jolly')
