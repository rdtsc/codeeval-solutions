#!/usr/bin/env python3

import sys

lines = list(filter(None, sys.stdin.read().split('\n')))

sliceSize = int(lines.pop(0))

lines.sort(key=len, reverse=True)

print(*lines[:sliceSize], sep='\n')
