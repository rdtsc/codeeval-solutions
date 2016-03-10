#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  const lhs = line[0].split(''),
        rhs = line[1].split(/([-+])/);

  const x = +lhs.slice(0, rhs[0].length).join(''),
        y = +lhs.slice(rhs[0].length).join('');

  console.log(rhs[1] === '+' ? x + y : x - y);
});
