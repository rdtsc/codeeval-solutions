#!/usr/bin/env python3

import sys
import collections

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    hitCount = collections.Counter(line)

    for c in line:
      if c.isalpha() and hitCount[c] == 1:
        print(c)
        break
