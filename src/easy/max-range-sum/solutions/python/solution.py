#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  values = [int(n) for n in re.split('[ ;]', line)]
  period = values.pop(0)

  maxGain = 0

  for i in range(len(values) - period + 1):
    localSum = sum(values[i:i + period])

    if localSum > maxGain:
      maxGain = localSum

  print(maxGain)
