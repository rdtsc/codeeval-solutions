#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/[^\d\s]/g, '')
             .split(' ')
             .map(Number)
             .sort((a, b) => a - b);

  let distances = [line[0]];

  for(let i = 1; i < line.length; ++i)
  {
    distances.push(line[i] - line[i - 1]);
  }

  console.log(distances.join(','));
});
