#!/usr/bin/env python3

import sys

print(*(max(line.rstrip().split(), key=len) for line in sys.stdin), sep='\n')
