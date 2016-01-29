#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    values = list(map(int, line.split()))
    margin = len(values)
    result = 0

    while True:
      traded = False
      margin = max(int(margin / 1.25), 1)

      for i in range(len(values) - margin):
        j = i + margin
        if values[i] > values[j]:
          traded = True
          values[i], values[j] = values[j], values[i]

      if not traded and margin == 1:
        break

      result += 1

    print(result)
