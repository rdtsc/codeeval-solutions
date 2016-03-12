#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function reduce(n)
{
  let result = 0;

  while(n)
  {
    result += Math.pow(n % 10, 2);
    n = Math.floor(n / 10);
  }

  return result;
}

function isHappy(n)
{
  let chain = new Set();

  for(n = reduce(n); n != 1;)
  {
    if(chain.has(n)) return false;

    chain.add(n);
    n = reduce(n);
  }

  return true;
}

lineReader.on('line', (line) =>
{
  console.log(+isHappy(+line));
});
