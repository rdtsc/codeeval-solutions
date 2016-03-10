#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping =
{
  '.-':    'A', '-...':  'B', '-.-.':  'C', '-..':   'D',
  '.':     'E', '..-.':  'F', '--.':   'G', '....':  'H',
  '..':    'I', '.---':  'J', '-.-':   'K', '.-..':  'L',
  '--':    'M', '-.':    'N', '---':   'O', '.--.':  'P',
  '--.-':  'Q', '.-.':   'R', '...':   'S', '-':     'T',
  '..-':   'U', '...-':  'V', '.--':   'W', '-..-':  'X',
  '-.--':  'Y', '--..':  'Z', '-----': '0', '.----': '1',
  '..---': '2', '...--': '3', '....-': '4', '.....': '5',
  '-....': '6', '--...': '7', '---..': '8', '----.': '9',
  '':      ' '
};

lineReader.on('line', (line) =>
{
  console.log(line.split(' ').map(code => mapping[code]).join(''));
});
