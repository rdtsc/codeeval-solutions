#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let result = [];

  for(let n = +line; n > 0; n = Math.trunc(n / 26))
  {
    result.unshift(String.fromCharCode('A'.charCodeAt(0) + (--n % 26)));
  }

  console.log(result.join(''));
});
