#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (word, mask) = line.split()
  word = list(word)

  for i, upper in enumerate(int(bit) for bit in mask):
    if upper: word[i] = word[i].upper()

  print(*word, sep='')
