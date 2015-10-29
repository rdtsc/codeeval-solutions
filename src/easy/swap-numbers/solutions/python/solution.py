#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  words = map(lambda word: word[-1] + word[1:-1] + word[0], line.split())

  print(*words, sep=' ')
