#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  names = [name for name in line.split(' ') if name != '|']
  target = int(names.pop())

  while len(names) != 1:
    names.pop(target % len(names) - 1)

  print(names[0])
