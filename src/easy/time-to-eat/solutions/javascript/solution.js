#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function toSeconds(timestamp)
{
  const ts = timestamp.split(':').map(Number);

  return ts[0] * 3600 + ts[1] * 60 + ts[2];
}

lineReader.on('line', (line) =>
{
  line = line.split(' ').sort((a, b) => toSeconds(b) - toSeconds(a));

  console.log(line.join(' '));
});
