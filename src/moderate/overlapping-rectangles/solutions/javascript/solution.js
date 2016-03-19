#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  const lhs = {x1: line[0], y1: line[1], x2: line[2], y2: line[3]},
        rhs = {x1: line[4], y1: line[5], x2: line[6], y2: line[7]};

  const x = lhs.x1 > rhs.x2 || lhs.x2 < rhs.x1,
        y = lhs.y1 < rhs.y2 || lhs.y2 > rhs.y1;

  console.log(x || y ? 'False' : 'True');
});
