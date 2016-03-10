#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  let frequencies = {};

  line.forEach((value) =>
  {
    frequencies[value] = frequencies[value] || 0;

    ++frequencies[value];
  });

  const threshold = Math.trunc(line.length / 2);

  for(const value in frequencies)
  {
    if(frequencies[value] > threshold)
    {
      console.log(value);
      return;
    }
  }

  console.log('None');
});
