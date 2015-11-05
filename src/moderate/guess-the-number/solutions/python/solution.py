#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  actions = line.split()

  guess = 0
  hi = int(actions.pop(0))
  lo = 0

  for action in actions:
    delta = hi - lo
    guess = lo + (delta & 1) + (delta // 2)

    if action.startswith('H'):
      lo = guess + 1

    elif action.startswith('L'):
      hi = guess - 1

  print(guess)
