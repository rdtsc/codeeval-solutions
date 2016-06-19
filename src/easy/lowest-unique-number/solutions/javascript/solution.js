#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  const players     = [],
        frequencies = Array(10).fill(0);

  line.split(' ').map(Number).forEach((value) =>
  {
    players.push(value);
    ++frequencies[value];
  });

  const winner = frequencies.findIndex(n => n === 1);

  if(winner < 0)
  {
    console.log(0);
    return;
  }

  console.log(players.findIndex(n => n === winner) + 1);
});
