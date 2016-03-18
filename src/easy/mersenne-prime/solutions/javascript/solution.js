#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const terms = [3, 7, 31, 127, 2047];

lineReader.on('line', (value) =>
{
  console.log(terms.filter(p => p < value).join(', '));
});
