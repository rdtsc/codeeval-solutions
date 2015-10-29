#!/usr/bin/env python3

from math import sqrt

def isPrime(n):
  if n <= 1:      return False
  elif n < 4:     return True
  elif not n & 1: return False
  elif n < 9:     return True
  elif not n % 3: return False

  limit = int(sqrt(n))

  i = 5

  while i <= limit:
    if not n % i or not n % (i + 2):
      return False
    i += 6

  return True

primeSum = 5
primeCount = 2
i = 5

while primeCount < 1000:
  if isPrime(i):
    primeSum += i
    primeCount += 1
  i += 1

print(primeSum)
