#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  (digits, expression) = line.split()

  op = re.search('[+-]', expression).start()

  lhs = int(digits[:op])
  rhs = int(digits[op:])

  op = expression[op]

  print(lhs + rhs if op == '+' else lhs - rhs)
