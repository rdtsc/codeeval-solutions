#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  frequencies = [0] * 26

  for c in filter(str.isalpha, map(str.lower, line)):
    frequencies[ord(c) - ord('a')] += 1

  frequencies.sort(reverse=True)

  multiplier = len(frequencies)
  score = 0

  for count in frequencies:
    score += multiplier * count
    multiplier -= 1

  print(score)
