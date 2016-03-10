#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.toLowerCase().split(/[^a-z]/).filter(word => word.length);

  console.log(line.join(' '));
});
