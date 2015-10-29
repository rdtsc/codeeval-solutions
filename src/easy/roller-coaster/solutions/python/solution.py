#!/usr/bin/env python3

import sys

actions = [str.upper, str.lower]

for line in (line.rstrip() for line in sys.stdin):
  line = list(line)

  action = 0

  for i, c in enumerate(line):
    if c.isalpha():
      line[i] = actions[action & 1](c)
      action += 1

  print(*line, sep='')
