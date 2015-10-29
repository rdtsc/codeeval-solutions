#!/usr/bin/env python3

import sys
import collections

def contains(string, fragments):
  letters = collections.Counter(string)

  return all(fragments[c] <= letters[c] for c in fragments)

for line in (line.rstrip() for line in sys.stdin):
  (names, fragments) = line.split('|')

  fragments = collections.Counter(fragments.strip())
  names = [name for name in names.split() if contains(name, fragments)]

  print(' '.join(names) if names else 'False')
