#!/usr/bin/env python3

key = 'OAGSFUKBCVWPJTEXYHDIQNLZMR'

message = '012222 1114142503 0313012513 03141418192' \
          '102 0113 2419182119021713 06131715070119'

payload = []

digit = lambda c: ord(c) - ord('0')

i = 0

while i < len(message):
  if message[i] == ' ':
    payload.append(' ')
    i += 1

  else:
    index = digit(message[i]) * 10 + digit(message[i + 1])
    payload.append(key[index])
    i += 2

print(''.join(payload))
