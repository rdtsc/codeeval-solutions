#!/usr/bin/env python3

import sys

print(*(line.lower() for line in sys.stdin), sep='')
