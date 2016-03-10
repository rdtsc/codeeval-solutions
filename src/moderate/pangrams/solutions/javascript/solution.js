#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/[^a-z]/ig, '').toLowerCase().split('');

  let history = Array(26).fill(false);

  line.forEach(c => history[c.charCodeAt(0) - 'a'.charCodeAt(0)] = true);

  let missing = [];

  history.forEach((value, index) =>
  {
    if(!value) missing.push(String.fromCharCode('a'.charCodeAt(0) + index));
  });

  console.log(missing.join('') || 'NULL');
});
