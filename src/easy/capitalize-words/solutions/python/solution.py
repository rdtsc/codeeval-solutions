#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  print(*map(lambda word: word[0].upper() + word[1:], line.split()))
