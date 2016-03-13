#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping = () =>
{
  let result = {};

  const keys   = 'abcdefghijklmnopqrstuvwxyz'.split(''),
        values = 'uvwxyznopqrstghijklmabcdef'.split('');

  keys.forEach((key, index) => result[key] = values[index]);

  return result;
}();

lineReader.on('line', (line) =>
{
  console.log(line.split('').map(c => mapping[c]).join(''));
});
