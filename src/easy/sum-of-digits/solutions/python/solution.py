#!/usr/bin/env python3

import sys

print(*(sum(map(int, str(line.rstrip()))) for line in sys.stdin), sep='\n')
