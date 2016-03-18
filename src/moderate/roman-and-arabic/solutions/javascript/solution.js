#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function toDecimal(literal)
{
  const romanBases =
  {
    'I': 1,  'V': 5,   'X': 10,
    'L': 50, 'C': 100, 'D': 500,
    'M': 1000
  };

  let result = 0;
  let previousBase = 1;

  for(let i = literal.length - 1; i > 0; i -= 2)
  {
    const r = romanBases[literal[i]],
          a = literal.charCodeAt(i - 1) - '0'.charCodeAt(0);

    result += previousBase > r ? -r * a : r * a;
    previousBase = r;
  }

  return result;
}

lineReader.on('line', line => console.log(toDecimal(line)));
