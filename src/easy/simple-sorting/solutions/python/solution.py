#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  values = sorted(map(float, line.split()))

  print(' '.join('{0:.3f}'.format(value) for value in values))
