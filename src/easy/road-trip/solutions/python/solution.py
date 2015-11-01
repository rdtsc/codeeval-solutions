#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  stops = sorted(int(n) for n in re.split('\D', line) if n)

  distances = \
  (
    stops[i] - stops[i - 1] if i else stops[i] for i in range(len(stops))
  )

  print(','.join(map(str, distances)))
