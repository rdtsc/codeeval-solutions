#!/usr/bin/env python3

import re
import sys
import colorsys

def hexToRgb(hex):
  patch = lambda lhs, rhs: int(hex[lhs:rhs], 16)

  return (patch(1, 3), patch(3, 5), patch(5, 7))

def cmykToRgb(cmyk):
  (c, m, y, k) = [float(n) for n in re.split(r'[^\d\.]', cmyk) if n]

  patch = lambda v: round((1 - v) * (1 - k) * 255)

  return (patch(c), patch(m), patch(y))

def hslToRgb(hsl):
  (h, s, l) = [int(n) for n in re.split('\D', hsl) if n]

  h /= 360
  s /= 100
  l /= 100

  patch = lambda v: round(v * 255)

  return tuple(patch(i) for i in colorsys.hls_to_rgb(h, l, s))

def hsvToRgb(hsv):
  (h, s, v) = [int(n) for n in re.split('\D', hsv) if n]

  h /= 360
  s /= 100
  v /= 100

  patch = lambda v: round(v * 255)

  return tuple(patch(i) for i in colorsys.hsv_to_rgb(h, s, v))

for line in (line.rstrip() for line in sys.stdin):
  (r, g, b) = hexToRgb(line)  if line[0] == '#' else \
              cmykToRgb(line) if line[0] == '(' else \
              hslToRgb(line)  if line[2] == 'L' else \
              hsvToRgb(line)  if line[2] == 'V' else (0, 0, 0)

  print('RGB({},{},{})'.format(r, g, b))
