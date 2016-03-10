#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ').map(chunk => chunk.split(' ').map(Number));

  line = line[0].map((value, index) =>
  {
    return line.map(row => row[index]);
  });

  line = line.map((row) => row.reduce((a, b) => a > b ? a : b));

  console.log(line.join(' '));
});
