#!/usr/bin/env python3

import sys
import string

mapping = str.maketrans(string.ascii_lowercase, 'yhesocvxduiglbkrztnwjpfmaq')

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(line.translate(mapping))
