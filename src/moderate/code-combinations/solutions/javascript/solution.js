#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const target = 'code'.split('').sort().join('');

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  const w = line[0].length,
        h = line.length;

  let matchCount = 0;

  for(let y = 0; y < h - 1; ++y)
  for(let x = 0; x < w - 1; ++x)
  {
    const sieve =
    [
      line[y + 0][x + 0],
      line[y + 0][x + 1],
      line[y + 1][x + 0],
      line[y + 1][x + 1]
    ].sort().join('');

    matchCount += (sieve === target);
  }

  console.log(matchCount);
});
