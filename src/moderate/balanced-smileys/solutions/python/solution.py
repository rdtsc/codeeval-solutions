#!/usr/bin/env python3

import sys

def isBalanced(string):
  i = lhs = rhs = 0

  while i < len(string):
    noLeadingColon = (string[i - 1] if i else string[i]) != ':'

    if string[i] == ')':
      if noLeadingColon:
        lhs -= 1

        if lhs < 0:
          return False

      rhs -= 1

      if rhs < 0:
        rhs = 0

    elif string[i] == '(':
      lhs += 1

      if noLeadingColon:
        rhs += 1

    i += 1

  return not rhs and lhs >= 0

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print('YES' if isBalanced(line) else 'NO')
