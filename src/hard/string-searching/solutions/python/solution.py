#!/usr/bin/env python3

import re
import sys

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (haystack, needle) = line.split(',')

    needle = needle.replace('\*', ',').replace('*', '.*').replace(',', '\*')
    needle = '.*{}.*'.format(needle)

    print(str(re.match(needle, haystack) is not None).lower())
