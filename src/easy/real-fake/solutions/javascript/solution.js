#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/\s+/g, '').split('').map(Number);

  const isLhs = (value, index) => !(index % 2),
        isRhs = (value, index) => !isLhs(value, index);

  const lhs = line.filter(isLhs).map(n => n * 2).reduce((a, b) => a + b),
        rhs = line.filter(isRhs).reduce((a, b) => a + b);

  console.log((lhs + rhs) % 10 ? 'Fake' : 'Real');
});
