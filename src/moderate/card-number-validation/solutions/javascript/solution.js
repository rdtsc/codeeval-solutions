#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  const digits = line.replace(/\s/g, '').split('').map(Number).reverse();

  let checksum = 0;

  for(let i = 0; i < digits.length; ++i)
  {
    let digit = digits[i];

    if((i + 1) & 1) checksum += digit;

    else
    {
      digit *= 2;

      if(digit >= 10) digit -= 9;

      checksum += digit;
    }
  }

  console.log(+!(checksum % 10));
});
