#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' : ');

  let lhs = line[0].split(' '),
      rhs = line[1].split(',').map(n => n.split('-').map(Number));

  rhs.forEach((op) =>
  {
    let oldA = lhs[op[0]];

    lhs[op[0]] = lhs[op[1]];
    lhs[op[1]] = oldA;
  });

  console.log(lhs.join(' '));
});
