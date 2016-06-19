#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('|')[2]
             .split(' ')
             .filter(chunk => chunk.length)
             .map(Number);

  const encryptionOffset =
    (Math.min.apply(Math, line) - ' '.charCodeAt(0));

  line = line.map(n => String.fromCharCode(n - encryptionOffset));

  console.log(line.join(''));
});
