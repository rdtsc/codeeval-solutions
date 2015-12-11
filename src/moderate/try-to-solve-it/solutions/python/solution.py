#!/usr/bin/env python3

import sys
import string

mapping = str.maketrans(string.ascii_lowercase, 'uvwxyznopqrstghijklmabcdef')

print(*(line.rstrip().translate(mapping) for line in sys.stdin), sep='\n')
