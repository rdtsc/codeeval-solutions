#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getCategory(n)
{
  if(n < 0 || n > 100) return 'This program is for humans';

  return n <=  2 ? "Still in Mama's arms" :
         n <=  4 ? 'Preschool Maniac'     :
         n <= 11 ? 'Elementary school'    :
         n <= 14 ? 'Middle school'        :
         n <= 18 ? 'High school'          :
         n <= 22 ? 'College'              :
         n <= 65 ? 'Working for the man'  :
                   'The Golden Years';
}

lineReader.on('line', (line) =>
{
  console.log(getCategory(+line));
});
