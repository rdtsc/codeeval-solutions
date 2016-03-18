#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let names   = line.split(' '),
      pattern = names.shift();

  pattern = pattern.replace(/\./g, '\\.')
                   .replace(/\?/g, '.')
                   .replace(/\*/g, '.*?');

  pattern = new RegExp(`^${pattern}$`);

  names = names.filter((name) =>
  {
    return pattern.test(name);
  });

  console.log(names.join(' ') || '-');
});
