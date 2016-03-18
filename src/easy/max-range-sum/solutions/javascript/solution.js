#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  const values = line.split(/[ ;]/).map(Number),
        period = values.shift();

  let maxGain = 0;

  for(let i = 0, j = (values.length - period + 1); i < j; ++i)
  {
    let localSum = values.slice(i, i + period)
                         .reduce((a, b) => a + b);

    if(localSum > maxGain) maxGain = localSum;
  }

  console.log(maxGain);
});
