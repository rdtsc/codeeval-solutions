#!/usr/bin/env python3

import sys

mapping = str.maketrans('23456789TJQKA', '\2\3\4\5\6\7\10\11\12\13\14\15\16')

deflate = lambda string: string.replace('10', 'T')
inflate = lambda string: string.replace('T', '10')

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (lhs, rhs, trumpSuit) = deflate(line).replace('|', ' ').split()

    a = ord(lhs[0].translate(mapping)) << ((lhs[1] == trumpSuit) * 3)
    b = ord(rhs[0].translate(mapping)) << ((rhs[1] == trumpSuit) * 3)

    rhs = '' if a > b else inflate(rhs)
    lhs = '' if a < b else inflate(lhs)

    print(' '.join(filter(None, (lhs, rhs))))
