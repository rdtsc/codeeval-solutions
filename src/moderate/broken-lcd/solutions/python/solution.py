#!/usr/bin/env python3

import re
import sys

def encode(literal):
  digit = lambda c: \
  [
    0xFC, 0x60, 0xDA, 0xF2, 0x66,
    0xB6, 0xBE, 0xE0, 0xFE, 0xF6
  ][ord(c) - ord('0')]

  result = []

  for c in literal:
    if c.isdigit():
      result.append(digit(c))
    else:
      result[-1] += 1

  return result

def canDisplay(number, segments):
  if len(number) > len(segments):
    return False

  for i in range(len(segments) - len(number) + 1):
    isFit = True

    for j in range(i, i + len(number)):
      if segments[j] & number[j - i] != number[j - i]:
        isFit = False
        break

    if isFit:
      return True

  return False

for line in (line.rstrip() for line in sys.stdin):
  values = re.split('[ ;]', line)
  number = encode(values.pop())
  values = [int(n, 2) for n in values]

  print(int(canDisplay(number, values)))
