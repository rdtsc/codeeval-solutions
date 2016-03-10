#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function nCk(n, k)
{
  const f = (n) => n > 0 ? n * f(n - 1) : 1;

  return Math.trunc(f(n) / (f(k) * f(n - k)));
}

lineReader.on('line', (line) =>
{
  const n = +line;

  for(let d = 0; d <  n; ++d)
  for(let i = 0; i <= d; ++i)
  {
    process.stdout.write(`${nCk(d, i)} `);
  }

  process.stdout.write('\n');
});
