#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let values = line.split(/\D/).filter(chunk => chunk.length).map(Number);

  const iterations = values.pop();

  for(let i = 0; i < iterations; ++i)
  for(let j = 0; j < values.length - 1; ++j)
  {
    if(values[j] > values[j + 1])
    {
      const scratch = values[j];

      values[j] = values[j + 1];
      values[j + 1] = scratch;

      break;
    }
  }

  console.log(values.join(' '));
});
