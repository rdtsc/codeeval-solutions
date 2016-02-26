#!/usr/bin/env python3

import sys

def toCents(string):
  result = 0

  for c in string:
    if c.isdigit():
      result = (result * 10) + (ord(c) - ord('0'))

  if '.' not in string:
    result *= 100

  return result

tender = \
[
  (1, "PENNY"), (5, "NICKEL"), (10, "DIME"), (25, "QUARTER"),
  (50, "HALF DOLLAR"), (100, "ONE"), (200, "TWO"), (500, "FIVE"),
  (1000, "TEN"), (2000, "TWENTY"), (5000, "FIFTY"), (10000, "ONE HUNDRED")
]

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (price, cash) = map(toCents, line.split(';'))

    change = cash - price

    if price > cash:
      print('ERROR')

    elif price == cash:
      print('ZERO')

    else:
      i = len(tender) - 1

      while change > 0:
        if i > 0 and tender[i][0] > change:
          i -= 1

        else:
          change -= tender[i][0]
          print(tender[i][1], end=',' if change else '\n')
