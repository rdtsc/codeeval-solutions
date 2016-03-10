#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

let lines = slurp().split('\n').filter(line => line.length);

const sliceSize = lines.shift();

lines.sort((a, b) => b.length - a.length);

console.log(lines.slice(0, sliceSize).join('\n'));
