#!/usr/bin/env python3

import sys
import itertools

for line in (line.rstrip() for line in sys.stdin):
  (words, indices) = line.split(';')

  indices = [int(i) for i in indices.split()]

  words = \
  [
    list(pair) for pair in itertools.zip_longest(words.split(), indices)
  ]

  words[-1][1] = sum(range(1, len(words) + 1)) - sum(indices)

  print(*(word[0] for word in sorted(words, key=lambda pair: pair[1])))
