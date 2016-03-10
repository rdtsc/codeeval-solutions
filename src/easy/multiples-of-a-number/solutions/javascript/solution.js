#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  const x = line[0],
        n = line[1];

  let multiple = x >> Math.log2(n);

  if(multiple * n != x) ++multiple;

  console.log(n * multiple);
});
