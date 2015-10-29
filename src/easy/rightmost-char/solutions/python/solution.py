#!/usr/bin/env python3

import sys

for line in (line.rstrip() for line in sys.stdin):
  print(line[:-1].rfind(line[-1]))
