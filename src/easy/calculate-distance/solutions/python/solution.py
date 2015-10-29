#!/usr/bin/env python3

import re
import sys
import math

for line in (line.rstrip() for line in sys.stdin):
  (x0, y0, x1, y1) = map(int, filter(None, re.split('[\(\),\s]', line)))

  print(int(math.hypot(x1 - x0, y1 - y0)))
