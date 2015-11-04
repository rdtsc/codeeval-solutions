#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  items = [int(n) for n in line.split()]
  itemCount = 1

  for i in range(1, len(items)):
    if items[i] == items[i - 1]:
      itemCount += 1

    else:
      print(itemCount, items[i - 1], end=' ')
      itemCount = 1

  print(itemCount, items[-1])
