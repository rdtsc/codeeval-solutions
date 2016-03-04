#!/usr/bin/env python3

import sys

target = set('code')

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    cells = [row.strip() for row in line.split('|')]

    matchCount = 0

    for y in range(len(cells) - 1):
      for x in range(len(cells[0]) - 1):
        sieve = \
        {
          cells[  y  ][x], cells[  y  ][x + 1],
          cells[y + 1][x], cells[y + 1][x + 1]
        }

        matchCount += (sieve == target)

    print(matchCount)
