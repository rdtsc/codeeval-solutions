#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('');

  const c = line[0].charCodeAt(0),
        n = +line[1];

  const emit = (x, y) =>
  {
    const lhs = String.fromCharCode(c + x),
          rhs = (n + y).toString();

    process.stdout.write(`${lhs}${rhs} `);
  };

  if(c >= 'c'.charCodeAt(0))
  {
    if(n >= 2) emit(-2, -1);
    if(n <= 7) emit(-2, +1);
  }

  if(c >= 'b'.charCodeAt(0))
  {
    if(n >= 3) emit(-1, -2);
    if(n <= 6) emit(-1, +2);
  }

  if(c <= 'g'.charCodeAt(0))
  {
    if(n >= 3) emit(+1, -2);
    if(n <= 6) emit(+1, +2);
  }

  if(c <= 'f'.charCodeAt(0))
  {
    if(n >= 2) emit(+2, -1);
    if(n <= 7) emit(+2, +1);
  }

  process.stdout.write('\n');
});
