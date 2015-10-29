#!/usr/bin/env python3

import sys

for n in (float(n) for n in sys.stdin):
  deg = int(n)
  min = int((n - deg) * 60)
  sec = int(((n - deg) * 60 - min) * 60)

  print('{:}.{:02}\'{:02}"'.format(deg, min, sec))
