#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping =
{
  'ze': 0, 'on': 1, 'tw': 2, 'th': 3, 'fo': 4,
  'fi': 5, 'si': 6, 'se': 7, 'ei': 8, 'ni': 9
}

lineReader.on('line', (line) =>
{
  line = line.split(';').map(key => mapping[key.slice(0, 2)]);

  console.log(line.join(''));
});
