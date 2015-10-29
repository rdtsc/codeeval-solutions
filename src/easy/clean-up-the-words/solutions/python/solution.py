#!/usr/bin/env python3

import re
import sys

for line in (line.rstrip() for line in sys.stdin):
  split = lambda pattern: re.split(pattern, line, flags=re.IGNORECASE)

  print(*(chunk.lower() for chunk in split('[^a-z]') if chunk))
