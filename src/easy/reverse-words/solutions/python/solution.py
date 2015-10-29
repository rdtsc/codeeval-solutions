#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  print(' '.join(reversed(line.split())))
