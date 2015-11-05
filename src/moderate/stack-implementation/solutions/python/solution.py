#!/usr/bin/env python3

import sys

class Stack:
  values = []

  def __init__(self):
    self.values = []

  def push(self, value):
    self.values.append(value)

  def pop(self):
    return self.values.pop() if not self.empty() else None

  def empty(self):
    return not self.values

for line in (line.rstrip() for line in sys.stdin):
  stack = Stack()

  for value in line.split():
    stack.push(value)

  values = []

  while not stack.empty():
    values.append(stack.pop())
    stack.pop()

  print(' '.join(values))
