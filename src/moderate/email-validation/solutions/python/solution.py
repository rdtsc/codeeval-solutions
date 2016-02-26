#!/usr/bin/env python3

import re
import sys

wc   = '[a-z0-9]'
wcr  = '[a-z0-9-]+'
uca  = '[a-zA-Z0-9.!#$%&\'*+/=?^_`{|}~-]+'
ucb  = '[a-zA-Z0-9.!#$%&\'*+/=?^_`{|}~@-]+'
user = '({}|"{}")'.format(uca, ucb)
host = r'{0}({1}{0})?(\.{0}({1}{0})?)*'.format(wc, wcr)

email = re.compile('^{}@{}$'.format(user, host), re.IGNORECASE)

with open(sys.argv[1]) as file:
  for line in (line.rstrip() for line in file):
    print(str(email.match(line) is not None).lower())
