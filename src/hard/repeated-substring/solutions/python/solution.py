#!/usr/bin/env python3

import re
import sys

regex = re.compile(r'(.+)(?=.*\1)')
legit = lambda string: string != ' '

for line in (line.rstrip() for line in sys.stdin):
  matches = sorted(filter(legit, regex.findall(line)), key=len, reverse=True)

  print(matches[0] if matches else 'NONE')
