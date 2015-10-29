#!/usr/bin/env python3

import sys

def find(needle, haystack):
  return next((i for i, v in enumerate(haystack) if v == needle), 0)

for line in (line.rstrip() for line in sys.stdin):
  frequencies, players = [0] * 10, []

  for value in map(int, line.split()):
    frequencies[value] += 1
    players.append(value)

  lowestCard = find(1, frequencies)

  print(find(lowestCard, players) + 1 if lowestCard else 0)
