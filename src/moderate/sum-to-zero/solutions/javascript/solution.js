#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',').map(Number);

  const termCount = line.length;

  let result = 0;

  for(let a = 0 + 0; a < termCount - 3; ++a)
  for(let b = a + 1; b < termCount - 2; ++b)
  for(let c = b + 1; c < termCount - 1; ++c)
  for(let d = c + 1; d < termCount - 0; ++d)
  {
    result += !(line[a] + line[b] + line[c] + line[d]);
  }

  console.log(result);
});
