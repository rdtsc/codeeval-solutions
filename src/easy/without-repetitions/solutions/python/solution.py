#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  print(re.sub(r'(.)\1+', r'\1', line))
