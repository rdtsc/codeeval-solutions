#!/usr/bin/env node

'use strict';

console.log(require('fs').statSync(process.argv[2]).size);
