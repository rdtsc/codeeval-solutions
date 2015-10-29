#!/usr/bin/env python3

import sys

mapping = lambda c: int(c) if c.isdigit() else 'abcdefghij'.find(c)

for line in (line.rstrip() for line in sys.stdin):
  digits = filter(lambda n: n >= 0, map(mapping, list(line)))
  digits = ''.join(map(str, digits))

  print(digits if digits else 'NONE')
