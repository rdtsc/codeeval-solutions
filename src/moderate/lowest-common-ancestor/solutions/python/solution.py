#!/usr/bin/env python3

import sys

node = lambda value, lhs=0, rhs=0: {'value': value, 'lhs': lhs, 'rhs': rhs}
leaf = node

NODES = \
[
  node(30, 1, 2),  #      0    |      30
  node(8, 3, 4),   #     / \   |     /  \
  node(52),        #    1   2  |    8   52
  leaf(3),         #   / \     |   / \
  node(20, 5, 6),  #  3   4    |  3  20
  leaf(10),        #     / \   |     / \
  leaf(29)         #    5   6  |    10 29
]

def getLowestAncestor(a, b):
  i = 0

  while True:
    value = NODES[i]['value']

    if a < value and b < value:
      i = NODES[i]['lhs']

    elif a > value and b > value:
      i = NODES[i]['rhs']

    else:
      return NODES[i]

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(getLowestAncestor(*map(int, line.split()))['value'])
