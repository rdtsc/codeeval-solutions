#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

const queries = slurp().split('\n').filter(line => line.length).map(Number);

const maxQuery = Math.max.apply(null, queries);

let terms = [0, 1];

for(let i = 2; i <= maxQuery; ++i)
{
  terms.push(terms[i - 1] + terms[i - 2]);
}

queries.forEach(n => console.log(terms[n]));
