#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  const width = Math.sqrt(line.length);

  let rotated = [];

  for(let i = 0; i < width + 0; ++i)
  for(let j = 1; j < width + 1; ++j)
  {
    rotated.push(line[line.length + i - width * j]);
  }

  console.log(rotated.join(' '));
});
