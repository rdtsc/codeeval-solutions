#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getCombinationCount(value, index, denominations)
{
  if(value <= 0 || !index) return +!value;

  const get = (value, index) =>
  {
    return getCombinationCount(value, index, denominations);
  };

  return get(value, index - 1) +
         get(value - denominations[index - 1], index);
}

let memo = {};

lineReader.on('line', (line) =>
{
  const n = +line;

  if(!(n in memo))
  {
    memo[n] = getCombinationCount(n, 5, [1, 5, 10, 25, 50]);
  }

  console.log(memo[n]);
});
