#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping =
[
  [1000, 'M'], [900, 'CM'], [500, 'D'], [400, 'CD'],
  [100,  'C'], [90,  'XC'], [50,  'L'], [40,  'XL'],
  [10,   'X'], [9,   'IX'], [5,   'V'], [4,   'IV'],
  [1,    'I']
];

lineReader.on('line', (line) =>
{
  let n = +line;
  let symbols = [];

  mapping.forEach((entry) =>
  {
    while(entry[0] <= n)
    {
      symbols.push(entry[1]);
      n -= entry[0];
    }
  });

  console.log(symbols.join(''));
});
