#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (alphabet, keys) = line.split('|')

  print(*(alphabet[int(key) - 1] for key in keys.split()), sep='')
