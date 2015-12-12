#!/usr/bin/env python3

import sys
import string

mapping = str.maketrans(string.ascii_lowercase, 'uvwxyznopqrstghijklmabcdef')

print(sys.stdin.read().translate(mapping))
