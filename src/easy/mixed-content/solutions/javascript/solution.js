#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  let words = [], numbers = [];

  line.forEach((chunk) =>
  {
    if(isNaN(chunk.charAt(0))) words.push(chunk);

    else numbers.push(chunk);
  });

  line = [];

  if(words.length) line.push(words.join(','));
  if(numbers.length) line.push(numbers.join(','));

  console.log(line.join('|'));
});
