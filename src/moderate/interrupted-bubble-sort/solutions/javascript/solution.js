#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(/\D/).filter(chunk => chunk.length).map(Number);

  const iterations = line.pop();

  for(let i = 0, j = Math.min(line.length, iterations); i < j; ++i)
  {
    for(let k = 0; k < line.length - 1; ++k)
    {
      if(line[k] > line[k + 1])
      {
        const scratch = line[k];
        line[k] = line[k + 1];
        line[k + 1] = scratch;
      }
    }
  }

  console.log(line.join(' '));
});
