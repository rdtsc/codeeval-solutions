#!/usr/bin/env python3

import sys

from urllib.parse import urlparse, unquote

for line in (line.rstrip() for line in sys.stdin):
  (lhs, rhs) = line.split(';')

  lhs = urlparse(unquote(lhs))
  rhs = urlparse(unquote(rhs))

  lhsPort = lhs.port or 80
  rhsPort = rhs.port or 80

  isSame = lhs.scheme   == rhs.scheme   and \
           lhs.hostname == rhs.hostname and \
           lhsPort      == rhsPort      and \
           lhs.path     == rhs.path

  print(isSame)
