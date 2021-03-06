#!/usr/bin/env python3

import sys

segments = \
[
  ['-**--', '*--*-', '*--*-', '*--*-', '-**--', '-----'],
  ['--*--', '-**--', '--*--', '--*--', '-***-', '-----'],
  ['***--', '---*-', '-**--', '*----', '****-', '-----'],
  ['***--', '---*-', '-**--', '---*-', '***--', '-----'],
  ['-*---', '*--*-', '****-', '---*-', '---*-', '-----'],
  ['****-', '*----', '***--', '---*-', '***--', '-----'],
  ['-**--', '*----', '***--', '*--*-', '-**--', '-----'],
  ['****-', '---*-', '--*--', '-*---', '-*---', '-----'],
  ['-**--', '*--*-', '-**--', '*--*-', '-**--', '-----'],
  ['-**--', '*--*-', '-***-', '---*-', '-**--', '-----']
]

for line in (line.rstrip() for line in sys.stdin):
  digits = [int(n) for n in line if n.isdigit()]

  for y in range(len(segments[0])):
    print(*(segments[digit][y] for digit in digits), sep='')
