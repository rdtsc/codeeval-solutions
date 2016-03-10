#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let deg = Math.trunc(+line),
      min = Math.trunc((+line - deg) * 60),
      sec = Math.trunc(((+line - deg) * 60 - min) * 60);

  if(min < 10) min = '0' + min;
  if(sec < 10) sec = '0' + sec;

  console.log(`${deg}.${min}'${sec}"`);
});
