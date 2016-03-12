#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').reverse();

  let values = [];

  line.forEach((token) =>
  {
    if(!isNaN(token))
    {
      values.push(+token);
      return;
    }

    let x = values.pop(),
        y = values.pop();

    switch(token)
    {
      case '*': values.push(x * y); break;
      case '+': values.push(x + y); break;
      case '/': values.push(x / y); break;
      case '-': values.push(x - y); break;
    }
  });

  console.log(Math.trunc(values[0]));
});
