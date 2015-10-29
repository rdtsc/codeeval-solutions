#!/usr/bin/env python3

import sys

classify = lambda age: "Still in Mama's arms" if age <=  2 else \
                       "Preschool Maniac"     if age <=  4 else \
                       "Elementary school"    if age <= 11 else \
                       "Middle school"        if age <= 14 else \
                       "High school"          if age <= 18 else \
                       "College"              if age <= 22 else \
                       "Working for the man"  if age <= 65 else \
                       "The Golden Years"

for age in (int(age) for age in sys.stdin):
  print('This program is for humans' if 0 < age > 100 else classify(age))
