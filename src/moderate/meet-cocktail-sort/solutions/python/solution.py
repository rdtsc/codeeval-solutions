#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip().replace('|', ' ') for line in file):
    values = list(map(int, line.split()))

    for i in range(values.pop()):

      for j in range(len(values) - 1):
        if values[j] > values[j + 1]:
          values[j], values[j + 1] = values[j + 1], values[j]

      for j in range(len(values) - 1, 0, -1):
        if values[j] < values[j - 1]:
          values[j], values[j - 1] = values[j - 1], values[j]

    print(*values, sep=' ')
