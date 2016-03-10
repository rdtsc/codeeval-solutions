#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(/\D/).slice(1).map(Number);

  const expectedSum = ((line.length - 1) * (line.length - 2)) >> 1;

  console.log(line.reduce((a, b) => a + b) - expectedSum);
});
