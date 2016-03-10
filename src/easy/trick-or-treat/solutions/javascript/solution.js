#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/[^\d\s]/g, '')
             .split(' ')
             .filter(chunk => chunk.length)
             .map(Number)

  const v = line[0], z = line[1],
        w = line[2], h = line[3];

  const loot   = ((v * 3) + (z * 4) + (w * 5)) * h,
        shares = (v + z + w);

  console.log(Math.trunc(loot / shares));
});
