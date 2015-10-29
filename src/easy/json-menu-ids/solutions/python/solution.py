#!/usr/bin/env python3

import sys
import json

for line in sys.stdin:
  items = json.loads(line)['menu']['items']

  ids = (item['id'] for item in items if item and 'label' in item)

  print(sum(ids))
