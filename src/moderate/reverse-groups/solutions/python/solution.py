#!/usr/bin/env python3

import re
import sys
import itertools

for line in (line.rstrip() for line in sys.stdin):
  items = [int(n) for n in re.split('\D', line)]
  width = items.pop()

  tuples = (pack[::-1] for pack in zip(*[iter(items)] * width))

  padding = items[-(len(items) % width):] if len(items) % width else []

  items = list(itertools.chain.from_iterable(tuples)) + padding

  print(','.join(str(n) for n in items))
