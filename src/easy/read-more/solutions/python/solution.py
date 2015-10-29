#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  if len(line) <= 55:
    print(line)
    continue

  line = line[:40]

  lastSpace = line.rfind(' ')

  if lastSpace >= 0:
   line = line[:lastSpace]

  print(line, '... <Read More>', sep='')
