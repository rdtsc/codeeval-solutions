#!/usr/bin/env python3

import sys

print(*(line.split()[-2] for line in sys.stdin), sep='\n');
