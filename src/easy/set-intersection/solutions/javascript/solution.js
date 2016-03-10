#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(';');

  let lhs = line[0].split(',').map(Number),
      rhs = line[1].split(',').map(Number);

  let intersection = [];

  while(lhs.length && rhs.length)
  {
    if(lhs[0] === rhs[0])
    {
      intersection.push(lhs.shift());
      rhs.shift();
    }

    else if(lhs[0] < rhs[0]) lhs.shift();

    else rhs.shift();
  }

  console.log(intersection.join(','));
});
