#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  let maxSum   = line[0],
      localMax = maxSum;

  for(let i = 1; i < line.length; ++i)
  {
    localMax = Math.max(line[i], localMax + line[i]);
    maxSum = Math.max(maxSum, localMax);
  }

  console.log(maxSum);
});
