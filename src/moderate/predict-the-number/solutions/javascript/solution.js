#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  console.log(((line >>> 0).toString(2).match(/1/g) || []).length % 3);
});
