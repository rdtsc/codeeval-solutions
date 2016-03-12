#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function nextPermutation(values)
{
  const width = values.length - 1;

  let i = width,
      j = width;

  while(i > 0 && values[i - 1] > values[i]) --i;

  if(!i) return false;

  for(--i; values[j] <= values[i]; --j);

  let stash = values[i];

  values[i] = values[j];
  values[j] = stash;

  for(++i, j = width; j > i; ++i, --j)
  {
    stash = values[i];
    values[i] = values[j];
    values[j] = stash;
  }

  return true;
}

lineReader.on('line', (line) =>
{
  line = line.split('').sort();

  process.stdout.write(line.join(''));

  while(nextPermutation(line))
  {
    process.stdout.write(',' + line.join(''));
  }

  process.stdout.write('\n');
});
