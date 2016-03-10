#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number);

  const x = line[0],
        y = line[1],
        n = line[2];

  line = [];

  for(let i = 1; i <= n; ++i)
  {
    const fizz = 'F'.repeat(!(i % x)),
          buzz = 'B'.repeat(!(i % y));

    line.push(fizz + buzz || i);
  }

  console.log(line.join(' '));
});
