#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getVariantCount(string)
{
  string = string.split('');

  const lhs = string.splice(0, string.length >> 1),
        rhs = string;

  let asteriskCount = 0;

  for(let i = 0; i < lhs.length; ++i)
  {
    const a = lhs[i],
          b = rhs[i];

    if(a === '*' && b === '*') ++asteriskCount;

    else if(a !== b && a !== '*' && b !== '*')
    {
      return 0;
    }
  }

  return Math.pow(2, asteriskCount);
}

lineReader.on('line', (line) =>
{
  console.log(getVariantCount(line));
});
