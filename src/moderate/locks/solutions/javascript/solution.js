#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number);

  const n = line[0],
        m = line[1];

  let locks = Array(n).fill(false);

  for(let i = 1; i < m; ++i)
  {
    for(let j = 1; j < n; j += 2) locks[j] = true;
    for(let j = 2; j < n; j += 3) locks[j] = !locks[j];
  }

  locks[n - 1] = !locks[n - 1];

  console.log(locks.filter(lock => !lock).length);
});
