#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isBalanced(line)
{
  if(line.length & 1) return false;

  let parentheses = [];

  for(let i = 0; i < line.length; ++i)
  {
    const c = line[i];

    if(c === '(' || c === '{' || c === '[') parentheses.push(c);

    else if(c === ')' || c === '}' || c === ']')
    {
      if(!parentheses.length) return false;

      const tail = parentheses[parentheses.length - 1];

      const offset = c === ')' ? 1 : 2;

      const complement = String.fromCharCode(tail.charCodeAt(0) + offset);

      if(complement !== c) return false;

      parentheses.pop();
    }
  }

  return !parentheses.length;
}

lineReader.on('line', (line) =>
{
  console.log(isBalanced(line) ? 'True' : 'False');
});
