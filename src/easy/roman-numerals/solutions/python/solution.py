#!/usr/bin/env python3

import sys

mapping = \
[
  (1000, 'M'), (900, 'CM'), (500, 'D'), (400, 'CD'),
  (100,  'C'), (90,  'XC'), (50,  'L'), (40,  'XL'),
  (10,   'X'), (9,   'IX'), (5,   'V'), (4,   'IV'),
  (1,    'I')
]

for n in (int(n) for n in sys.stdin):
  symbols = []

  for value, symbol in mapping:
    while value <= n:
      symbols.append(symbol)
      n -= value

  print(''.join(symbols))
