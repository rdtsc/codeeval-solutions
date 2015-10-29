#!/usr/bin/env python3

import sys

emit = lambda c: print(c, end='')

terms = \
[
  ', yeah!',
  ', this is crazy, I tell ya.',
  ', can U believe this?',
  ', eh?',
  ', aw yea.',
  ', yo.',
  '? No way!',
  '. Awesome!'
]

injectSlang = False
termCount = len(terms)
term = 0

for i, c in enumerate(sys.stdin.read()):
  if c not in '.!?':
    emit(c)

  else:
    if not injectSlang:
      emit(c)

    else:
      emit(terms[term % termCount])
      term += 1

    injectSlang = not injectSlang
