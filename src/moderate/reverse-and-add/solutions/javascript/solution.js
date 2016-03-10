#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function reverse(n)
{
  let result = 0;

  for(;n; n = Math.trunc(n / 10))
  {
    result = (result * 10) + (n % 10);
  }

  return result;
}

lineReader.on('line', (line) =>
{
  let stepCount = 1;
  let m = +line + reverse(+line);

  while(m != reverse(m))
  {
    ++stepCount;
    m += reverse(m);
  }

  console.log(stepCount, m);
});
