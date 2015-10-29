#!/usr/bin/env python3

import sys

print(*(line.rstrip().swapcase() for line in sys.stdin), sep='\n');
