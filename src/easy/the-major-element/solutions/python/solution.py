#!/usr/bin/env python3

import sys
import collections

for line in (line.rstrip() for line in sys.stdin):
  frequency = collections.defaultdict(int)

  for value in line.split(','):
    frequency[value] += 1

  majorTerm = 'None'
  threshold = (line.count(',') + 1) // 2

  for key, count in frequency.items():
    if count > threshold:
      majorTerm = key
      break

  print(majorTerm)
