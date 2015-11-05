#!/usr/bin/env python3

import sys

for n in (int(n) for n in sys.stdin):
  print(bin(n)[2:])
