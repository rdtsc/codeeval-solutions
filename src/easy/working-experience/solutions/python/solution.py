#!/usr/bin/env python3

import sys

EPOCH_YEAR = 1990

def getEpochOffset(monthYear):
  (month, year) = monthYear.split()

  month = \
  {
    'jan': 1, 'feb':  2, 'mar':  3, 'apr':  4,
    'may': 5, 'jun':  6, 'jul':  7, 'aug':  8,
    'sep': 9, 'oct': 10, 'nov': 11, 'dec': 12
  }[month.lower()]

  year = int(year)

  return (year - EPOCH_YEAR) * 12 + month

for line in (line.rstrip() for line in sys.stdin):
  experience = [False] * ((2020 - EPOCH_YEAR) * 12)

  for date in (range.split('-') for range in line.split(';')):
    lhs = getEpochOffset(date[0])
    rhs = getEpochOffset(date[1])

    for month in range(lhs, rhs + 1):
      experience[month] = True

  print(experience.count(True) // 12)
