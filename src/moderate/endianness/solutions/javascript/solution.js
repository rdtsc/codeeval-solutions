#!/usr/bin/env node

'use strict';

console.log(require('os').endianness() == 'BE' ? 'BigEndian' : 'LittleEndian');
