#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const segments =
[
  ['-**--', '*--*-', '*--*-', '*--*-', '-**--', '-----'],
  ['--*--', '-**--', '--*--', '--*--', '-***-', '-----'],
  ['***--', '---*-', '-**--', '*----', '****-', '-----'],
  ['***--', '---*-', '-**--', '---*-', '***--', '-----'],
  ['-*---', '*--*-', '****-', '---*-', '---*-', '-----'],
  ['****-', '*----', '***--', '---*-', '***--', '-----'],
  ['-**--', '*----', '***--', '*--*-', '-**--', '-----'],
  ['****-', '---*-', '--*--', '-*---', '-*---', '-----'],
  ['-**--', '*--*-', '-**--', '*--*-', '-**--', '-----'],
  ['-**--', '*--*-', '-***-', '---*-', '-**--', '-----']
];

lineReader.on('line', (line) =>
{
  const digits = line.split(/\D/).join('').split('').map(Number);

  for(let i = 0; i < segments[0].length; ++i)
  {
    for(let j = 0; j < digits.length; ++j)
    {
      process.stdout.write(segments[digits[j]][i]);
    }

    process.stdout.write('\n');
  }
});
