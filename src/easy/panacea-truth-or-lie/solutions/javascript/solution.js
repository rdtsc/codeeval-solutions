#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  let lhs = line[0].split(' ').map(n => parseInt(n, 16)),
      rhs = line[1].split(' ').map(n => parseInt(n,  2));

  lhs = lhs.reduce((a, b) => a + b);
  rhs = rhs.reduce((a, b) => a + b);

  console.log(lhs <= rhs ? 'True' : 'False');
});
