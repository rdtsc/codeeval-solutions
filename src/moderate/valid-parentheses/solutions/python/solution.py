#!/usr/bin/env python3

import sys

def isBalanced(expression):
  if len(expression) & 1:
    return False

  parentheses = []

  for c in expression:
    if c == '(' or c == '{' or c == '[':
      parentheses.append(c)

    elif c == ')' or c == '}' or c == ']':
      offset = 1 if c == ')' else 2

      if len(parentheses) and chr(ord(parentheses[-1]) + offset) == c:
        parentheses.pop()

      else:
        return False

  return not parentheses

print(*(isBalanced(line.rstrip()) for line in sys.stdin), sep='\n')
