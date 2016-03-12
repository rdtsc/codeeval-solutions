#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  const haystack = line[0];

  let needle = line[1].replace(/\\\*/g, ',')
                      .replace(/\*/g, '.*')
                      .replace(/,/g, '\\*');

  needle = '.*' + needle + '.*';

  console.log(!!haystack.match(RegExp(needle)));
});
