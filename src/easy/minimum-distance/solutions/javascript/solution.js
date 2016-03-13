#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let values = line.split(' ').map(Number);

  const friendCount = values.shift();

  values.sort((a, b) => a - b);

  const midpoint = friendCount >> 1;
  let ourAddress = values[midpoint];

  if(!(friendCount & 1))
  {
    ourAddress = (values[midpoint - 1] + values[midpoint]) >> 1;
  }

  values = values.map(value => Math.abs(value - ourAddress));

  console.log(values.reduce((a, b) => a + b));
});
