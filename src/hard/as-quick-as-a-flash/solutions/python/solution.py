#!/usr/bin/env python3

import sys

def partition(values, head, tail):
  pivot = values[head]

  while True:
    while values[head] < pivot: head += 1
    while values[tail] > pivot: tail -= 1

    if head >= tail:
      return tail

    values[head], values[tail] = values[tail], values[head]

def quickSort(values, head, tail, index=0):
  if head >= tail:
    return index

  pivot = partition(values, head, tail)
  lhs   = quickSort(values, head, pivot - 1, index + 1)
  rhs   = quickSort(values, pivot + 1, tail)

  return lhs + rhs

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    line = [int(n) for n in line.split()]
    print(quickSort(line, 0, len(line) - 1))
