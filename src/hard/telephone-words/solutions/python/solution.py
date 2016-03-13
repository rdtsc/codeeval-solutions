#!/usr/bin/env python3

import sys

keypad = \
[
  ['0'],
  ['1'],
  ['a', 'b', 'c',],
  ['d', 'e', 'f',],
  ['g', 'h', 'i',],
  ['j', 'k', 'l',],
  ['m', 'n', 'o',],
  ['p', 'q', 'r', 's'],
  ['t', 'u', 'v',],
  ['w', 'x', 'y', 'z']
];

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    digits = [int(n) for n in line]

    isFirstResult = True

    for c0 in keypad[digits[0]]:
      for c1 in keypad[digits[1]]:
        for c2 in keypad[digits[2]]:
          for c3 in keypad[digits[3]]:
            for c4 in keypad[digits[4]]:
              for c5 in keypad[digits[5]]:
                for c6 in keypad[digits[6]]:

                  if isFirstResult:
                    isFirstResult = False

                  else:
                    print(',', end='')

                  print(c0, c1, c2, c3, c4, c5, c6, sep='', end='')

    print()
