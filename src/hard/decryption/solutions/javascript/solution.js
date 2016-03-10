#!/usr/bin/env node

'use strict';

const key = 'OAGSFUKBCVWPJTEXYHDIQNLZMR';

const message = '012222 1114142503 0313012513 03141418192' +
                '102 0113 2419182119021713 06131715070119';

const digit = (c) => c.charCodeAt(0) - '0'.charCodeAt(0);

let payload = [];

for(let i = 0; i < message.length; ++i)
{
  if(message[i] === ' ')
  {
    payload.push(' ');
    continue;
  }

  const index = digit(message[i]) * 10 + digit(message[i + 1]);
  payload.push(key[index]);
  ++i;
}

console.log(payload.join(''));
