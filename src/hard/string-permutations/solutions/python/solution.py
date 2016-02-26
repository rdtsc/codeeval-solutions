#!/usr/bin/env python3

import sys
import itertools

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(','.join(''.join(c) for c in itertools.permutations(sorted(line))))
