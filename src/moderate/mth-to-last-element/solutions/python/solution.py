#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  values = line.split()
  index = int(values.pop())

  if index <= len(values):
    print(values[len(values) - index])
