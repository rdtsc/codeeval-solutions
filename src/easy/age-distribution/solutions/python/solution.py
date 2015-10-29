#!/usr/bin/env python3

import sys

category = \
[
  "This program is for humans",
  "Still in Mama's arms",
  "Preschool Maniac",
  "Elementary school",
  "Middle school",
  "High school",
  "College",
  "Working for the man",
  "The Golden Years"
]

classify = lambda age: category[0] if age <   0 else \
                       category[0] if age > 100 else \
                       category[1] if age <=  2 else \
                       category[2] if age <=  4 else \
                       category[3] if age <= 11 else \
                       category[4] if age <= 14 else \
                       category[5] if age <= 18 else \
                       category[6] if age <= 22 else \
                       category[7] if age <= 65 else \
                       category[8]

print(*(map(classify, (int(n) for n in sys.stdin))), sep='\n')
