#!/usr/bin/env python3

import sys

queries = [int(n) for n in sys.stdin]

terms = [0, 1]

for i in range(2, max(queries) + 1):
  terms.append(terms[i - 1] + terms[i - 2])

print(*(terms[n] for n in queries), sep='\n')
