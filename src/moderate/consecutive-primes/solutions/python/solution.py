#!/usr/bin/env python3

import sys

terms = [1, 2, 2, 4, 96, 1024, 2880, 81024, 770144]

for n in (int(n) for n in sys.stdin):
  print(0 if not n or n & 1 else terms[n // 2 - 1])
