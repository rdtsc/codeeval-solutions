#!/usr/bin/env python3

import re
import sys

def bubbleSort(values, iterations):
  for i in range(min(len(values), iterations)):
    for j in range(len(values) - 1):
      if values[j] > values[j + 1]:
        values[j], values[j + 1] = values[j + 1], values[j]

  return values

for line in (line.rstrip() for line in sys.stdin):
  values = [int(value) for value in re.split('\D', line) if value]
  iterations = values.pop()

  print(*bubbleSort(values, iterations))
