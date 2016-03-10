#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  let lhs = line[0].split(':').map(Number),
      rhs = line[1].split(':').map(Number);

  lhs = lhs[0] * 3600 + lhs[1] * 60 + lhs[2];
  rhs = rhs[0] * 3600 + rhs[1] * 60 + rhs[2];

  const ds = Math.abs(lhs - rhs);

  let hh = Math.trunc(ds / 3600),
      mm = Math.trunc(ds / 60 % 60),
      ss = (ds % 60);

  if(hh < 10) hh = '0' + hh;
  if(mm < 10) mm = '0' + mm;
  if(ss < 10) ss = '0' + ss;

  console.log(`${hh}:${mm}:${ss}`);
});
