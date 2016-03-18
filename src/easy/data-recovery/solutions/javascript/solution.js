#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(';');

  let words   = line[0].split(' '),
      indices = line[1].split(' ').map(Number);

  let pairs = words.map((word, index) => [word, indices[index]]);

  let missingIndex = (words.length / 2) * (words.length + 1);

  missingIndex -= indices.reduce((a, b) => a + b);

  pairs[pairs.length - 1][1] = missingIndex;

  pairs.sort((a, b) => a[1] - b[1]);

  console.log(pairs.map(pair => pair[0]).join(' '));
});
