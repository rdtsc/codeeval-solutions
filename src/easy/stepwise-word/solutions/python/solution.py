#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  target = list(max(line.split(), key=len))

  for i, c in enumerate(target):
    target[i] = '*'*i + c

  print(*target)
