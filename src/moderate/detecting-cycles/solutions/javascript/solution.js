#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').reverse();

  let history = {};
  let duplicates = [];

  line.forEach((n) =>
  {
    if(!(n in history)) history[n] = true;

    else if(duplicates.indexOf(n) === -1)
    {
      duplicates.unshift(n);
    }
  });

  console.log(duplicates.join(' '));
});
