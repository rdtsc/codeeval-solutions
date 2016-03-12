#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  const size = +line[0];

  const pool = line[1].split('').sort().filter((value, i, pool) =>
  {
    return value != pool[i - 1];
  });

  () =>
  {
    let mask = Array(size).fill(0);

    for(;;)
    {
      mask.forEach(i => process.stdout.write(pool[i]));

      for(let i = size - 1;; --i)
      {
        if(i < 0) return;

        else if(++mask[i] === pool.length) mask[i] = 0;

        else
        {
          process.stdout.write(',');
          break;
        }
      }
    }
  }();

  process.stdout.write('\n');
});
