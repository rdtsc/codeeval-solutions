#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(', ');

  const pattern = new RegExp(`[${line[1]}]`, 'g');

  console.log(line[0].replace(pattern, ''));
});
