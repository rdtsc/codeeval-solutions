#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const keypad =
[
  ['0'],
  ['1'],
  ['a', 'b', 'c',],
  ['d', 'e', 'f',],
  ['g', 'h', 'i',],
  ['j', 'k', 'l',],
  ['m', 'n', 'o',],
  ['p', 'q', 'r', 's'],
  ['t', 'u', 'v',],
  ['w', 'x', 'y', 'z']
];

lineReader.on('line', (line) =>
{
  let isFirstResult = true;

  keypad[line[0]].forEach((c0) =>
  keypad[line[1]].forEach((c1) =>
  keypad[line[2]].forEach((c2) =>
  keypad[line[3]].forEach((c3) =>
  keypad[line[4]].forEach((c4) =>
  keypad[line[5]].forEach((c5) =>
  keypad[line[6]].forEach((c6) =>
  {
    if(isFirstResult) isFirstResult = false;

    else process.stdout.write(',');

    process.stdout.write([c0, c1, c2, c3, c4, c5, c6].join(''));
  })))))));

  process.stdout.write('\n');
});
