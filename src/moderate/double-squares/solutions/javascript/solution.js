#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

let skippedHeader = false;

lineReader.on('line', (line) =>
{
  if(!skippedHeader)
  {
    skippedHeader = true;
    return;
  }

  const n = +line;

  const ceiling = Math.trunc(Math.sqrt(n)) + 1;

  let result = 0;

  for(let i = 0; i < ceiling; ++i)
  {
    let value = n - i * i,
        test  = Math.trunc(Math.sqrt(value));

    if(test < i) break;

    if(test * test === value) ++result;
  }

  console.log(result);
});
