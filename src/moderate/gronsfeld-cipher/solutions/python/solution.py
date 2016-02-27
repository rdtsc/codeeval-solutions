#!/usr/bin/env python3

import sys

alphabet = ' !"#$%&\'()*+,-./0123456789:<=>?@ABCDEFGHI'\
           'JKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    (key, message) = line.split(';')

    i = 0
    result = []

    for c in message:
      j = ord(key[i]) - ord('0')
      k = alphabet.find(c)

      k = k - j if k >= j else len(alphabet) - (j - k)
      i = i + 1 if i < len(key) - 1 else 0

      result.append(alphabet[k])

    print(''.join(result))
