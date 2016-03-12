#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const wc   = '[a-z0-9]';
const wcr  = '[a-z0-9-]+';
const uca  = '[a-zA-Z0-9.!#$%&\'*+/=?^_`{|}~-]+';
const ucb  = '[a-zA-Z0-9.!#$%&\'*+/=?^_`{|}~@-]+';
const user = `(${uca}|"${ucb}")`;
const host = `${wc}(${wcr}${wc})?(\\.${wc}(${wcr}${wc})?)*`;

const email = RegExp(`^${user}@${host}$`, 'i');

lineReader.on('line', (line) =>
{
  console.log(!!line.match(email));
});
