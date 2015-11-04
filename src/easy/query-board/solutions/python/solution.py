#!/usr/bin/env python3

import sys

BOARD_WIDTH = 256
board = [[0] * BOARD_WIDTH for i in range(BOARD_WIDTH)]

for line in (line.rstrip() for line in sys.stdin):
  if line.startswith('Query'):
    (command, index) = line.split()
    index = int(index)

    if command == 'QueryRow':
      print(sum(board[index]))

    elif command == 'QueryCol':
      print(sum(board[i][index] for i in range(BOARD_WIDTH)))

  elif line.startswith('Set'):
    (command, index, value) = line.split()
    index = int(index)
    value = int(value)

    if command == 'SetRow':
      board[index] = [value for i in range(BOARD_WIDTH)]

    elif command == 'SetCol':
      for i in range(BOARD_WIDTH):
        board[i][index] = value
