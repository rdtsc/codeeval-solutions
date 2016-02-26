#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    terms = list(map(int, line.split(',')))
    termCount = len(terms)

    result = 0

    for i in range(termCount - 3):
      for j in range(i + 1, termCount - 2):
        for k in range(j + 1, termCount - 1):
          for l in range(k + 1, termCount):
            result += not terms[i] + terms[j] + terms[k] + terms[l]

    print(result)
