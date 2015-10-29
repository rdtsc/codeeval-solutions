#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  print(len(re.findall('(?=<--<<|>>-->)', line)))
