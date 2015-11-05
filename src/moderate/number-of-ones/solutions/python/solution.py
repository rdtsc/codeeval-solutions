#!/usr/bin/env python3

import sys

print(*(bin(int(n)).count('1') for n in sys.stdin), sep='\n')
