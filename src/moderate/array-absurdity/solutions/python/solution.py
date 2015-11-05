#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  terms = [int(n) for n in re.split('\D', line)]

  termCount = terms.pop(0)
  expectedArraySum = (termCount - 1) * (termCount - 2) // 2

  print(sum(terms) - expectedArraySum)
