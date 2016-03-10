#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

console.log(slurp().split('\n').map(Number).reduce((a, b) => a + b));
