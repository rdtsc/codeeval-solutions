#!/usr/bin/env python3

import sys

track = [list(line.rstrip()) for line in sys.stdin]

lastPos = track[0].index('_')

track[0][lastPos] = '|'

for i in range(1, len(track)):
  pos = track[i].index('C') if 'C' in track[i] else track[i].index('_')

  if lastPos == pos:
    track[i][pos] = '|'

  else:
    track[i][pos] = '/' if lastPos > pos else '\\'

  lastPos = pos

for segment in track:
  print(*segment, sep='')
