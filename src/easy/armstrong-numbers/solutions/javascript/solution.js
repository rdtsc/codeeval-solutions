#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  const n = +line;
  const digitCount = line.length;

  line = line.split('').map(d => Math.pow(+d, digitCount));
  line = line.reduce((a, b) => a + b);

  console.log(line === n ? 'True' : 'False');
});
