#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  (c, n) = list(line)

  n = int(n)

  emit = lambda x, y: print(chr(ord(c) + x), n + y, sep='', end=' ')

  if c >= 'c':
    if n >= 2: emit(-2, -1)
    if n <= 7: emit(-2, +1)

  if c >= 'b':
    if n >= 3: emit(-1, -2)
    if n <= 6: emit(-1, +2)

  if c <= 'g':
    if n >= 3: emit(+1, -2)
    if n <= 6: emit(+1, +2)

  if c <= 'f':
    if n >= 2: emit(+2, -1)
    if n <= 7: emit(+2, +1)

  print()
