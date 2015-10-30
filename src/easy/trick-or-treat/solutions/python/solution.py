#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  cols = [int(n) for n in re.split('\D', line) if n]
  loot = sum(x * y * cols[3] for x, y in zip(cols, [3, 4, 5]))

  print(loot // sum(cols[:-1]))
