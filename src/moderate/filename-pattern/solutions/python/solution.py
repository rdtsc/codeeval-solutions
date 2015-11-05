#!/usr/bin/env python3

import sys
import fnmatch

for line in (line.rstrip() for line in sys.stdin):
  names = line.split()
  pattern = names.pop(0)

  names = [name for name in names if fnmatch.fnmatch(name, pattern)]

  print(*names if names else '-')
