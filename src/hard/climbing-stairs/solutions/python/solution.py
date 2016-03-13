#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as file:
  queries = [int(n) for n in file]

terms = [0, 1]

for i in range(2, max(queries) + 2):
  terms.append(terms[i - 1] + terms[i - 2])

print(*(terms[n + 1] for n in queries), sep='\n')
