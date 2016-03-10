#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  const lhs = line[0],
        rhs = line[1];

  console.log(+(lhs.indexOf(rhs, lhs.length - rhs.length) !== -1));
});
