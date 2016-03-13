#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping =
{
  '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8,
  '9': 9, 'T': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14
};

const deflate = (string) => string.replace('10', 'T');
const inflate = (string) => string.replace('T', '10');

lineReader.on('line', (line) =>
{
  line = deflate(line).split(/[\s|]/).filter(chunk => chunk.length);

  let lhs = line[0],
      rhs = line[1];

  const trumpSuit = line[2];

  const a = mapping[lhs[0]] << ((lhs[1] === trumpSuit) * 3);
  const b = mapping[rhs[0]] << ((rhs[1] === trumpSuit) * 3);

  const result =
  [
    a < b ? '' : inflate(lhs),
    a > b ? '' : inflate(rhs)
  ].filter(Boolean);

  console.log(result.join(' '));
});
