#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number);

  const lhs = line[0],
        rhs = line[1];

  let tally = 0;

  for(let i = 1; i <= rhs; ++i)
  {
    const count = i.toString(2).split('0').length - 1;

    tally += (count === lhs);
  }

  console.log(tally);
});
