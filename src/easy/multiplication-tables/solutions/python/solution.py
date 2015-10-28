#!/usr/bin/env python3

scope = range(1, 13)
width = len(scope)
cells = (x * y for x in scope for y in scope)
line  = '{}' + ('{:>4}'*(width - 1)) + '\n'

print((line * width).format(*cells), end='')
