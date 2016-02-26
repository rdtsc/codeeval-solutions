#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    values = line.split()
    history = set()
    duplicates = []

    for i in reversed(values):
      if i not in history:
        history.add(i)

      elif i not in duplicates:
        duplicates.insert(0, i)

    print(' '.join(duplicates))
