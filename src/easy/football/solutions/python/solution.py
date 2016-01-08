#!/usr/bin/env python3

import sys
import collections

for line in (line.rstrip() for line in sys.stdin):
  countries = [map(int, teams.split()) for teams in line.split('|')]

  results = collections.defaultdict(list)

  for country, teams in enumerate(countries, start=1):
    for team in teams:
      results[team].append(str(country))

  results = sorted(results.items())
  results = '; '.join(str(lhs) + ':' + ','.join(rhs) for lhs, rhs in results)

  print(results, ';', sep='')
