#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let result = 0;
  let n = +line;

  [5, 3].forEach((denomination) =>
  {
    result += Math.trunc(n / denomination);
    n -= Math.trunc(n / denomination) * denomination;
  });

  console.log(result + n);
});
