#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  let lhs = line[0].split(' ').map(Number),
      rhs = line[1].split(' ').map(Number);

  lhs = lhs.map((value, index) => value * rhs[index]);

  console.log(lhs.join(' '));
});
