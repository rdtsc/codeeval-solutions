#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  print(*sorted(set(map(int, line.split(',')))), sep=',')
