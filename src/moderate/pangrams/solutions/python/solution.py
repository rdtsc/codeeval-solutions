#!/usr/bin/env python3

import sys
import string
import collections

for line in (line.rstrip() for line in sys.stdin):
  letters = collections.Counter(line.lower())
  missing = [c for c in string.ascii_lowercase if c not in letters]

  print(''.join(missing) if missing else 'NULL')
