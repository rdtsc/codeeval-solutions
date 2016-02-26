#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    terms = list(map(int, line.replace(',', ' ').replace(';', ' ').split()))
    targetSum = terms.pop()

    if len(terms) >= 2 and terms[-1] + terms[-2] < targetSum:
      print('NULL')

    else:
      pairs = []

      for i in range(len(terms)):
        for j in range(i + 1, len(terms)):
          pairSum = terms[i] + terms[j]

          if pairSum > targetSum:
            break

          if pairSum == targetSum:
            pairs.append((terms[i], terms[j]))

      if not pairs:
        print('NULL')

      else:
        print(';'.join('{},{}'.format(n[0], n[1]) for n in pairs))
