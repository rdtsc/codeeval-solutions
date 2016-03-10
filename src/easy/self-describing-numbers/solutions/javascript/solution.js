#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isSelfDescribing(n)
{
  const digits = n.toString().split('');
  let frequencies = Array.apply(null, Array(10)).map(() => 0);

  digits.forEach(digit => ++frequencies[digit]);

  return digits.every((digit, index) =>
  {
    return frequencies[index] == digit;
  });
}

lineReader.on('line', (line) =>
{
  console.log(isSelfDescribing(+line) ? 1 : 0);
});
