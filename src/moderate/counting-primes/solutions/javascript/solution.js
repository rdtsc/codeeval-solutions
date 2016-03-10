#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isPrime(n)
{
  if(n <= 1)        return false;
  else if(n < 4)    return true;
  else if(!(n & 1)) return false;
  else if(n < 9)    return true;
  else if(!(n % 3)) return false;

  const limit = Math.trunc(Math.sqrt(n));

  for(let i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
  }

  return true;
}

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  const n = line[0],
        m = line[1];

  let tally = 0;

  for(let i = n; i <= m; ++i)
  {
    tally += isPrime(i);
  }

  console.log(tally);
});
