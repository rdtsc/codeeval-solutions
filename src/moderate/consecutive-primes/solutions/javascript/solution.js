#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const terms = [1, 2, 2, 4, 96, 1024, 2880, 81024, 770144];

lineReader.on('line', (line) =>
{
  const n = +line;

  console.log(!n || n & 1 ? 0 : terms[(n >> 1) - 1]);
});
