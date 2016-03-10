#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

const queries = slurp().split('\n').filter(line => line.length).map(Number);

const maxQuery = Math.max.apply(null, queries);

let isPrime = Array(maxQuery).fill(true);

const limit = Math.trunc(Math.sqrt(maxQuery)) + 1;

for(let i = 2; i < limit; ++i)
{
  if(isPrime[i])
  {
    for(let j = i * i; j < maxQuery; j += i)
    {
      isPrime[j] = false;
    }
  }
}

queries.forEach((value) =>
{
  let terms = [];

  for(let i = 2; i < value; ++i) if(isPrime[i])
  {
    terms.push(i);
  }

  console.log(terms.join(','));
});
