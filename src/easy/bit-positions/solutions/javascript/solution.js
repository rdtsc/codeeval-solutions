#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  const n = line[0].toString(2),
        x = line[1],
        y = line[2];

  console.log(n[n.length - x] === n[n.length - y] ? 'true' : 'false');
});
