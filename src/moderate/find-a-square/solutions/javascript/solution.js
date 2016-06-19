#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isSquare(distances)
{
  if(distances[4] !== distances[5]) return false;

  for(let i = 0; i < 4; ++i)
  {
    if(!distances[i] || distances[i] !== distances[0]) return false;
  }

  return true;
}

function toPairs(result, value, index, values)
{
  if(!(index & 1)) result.push(values.slice(index, (index + 2)));

  return result;
}

function getDistance(lhs, rhs)
{
  return Math.hypot((lhs[0] - rhs[0]), (lhs[1] - rhs[1]));
}

lineReader.on('line', (line) =>
{
  const points = line.replace(/\D/g, ' ')
                     .split(' ')
                     .filter(chunk => chunk)
                     .map(Number)
                     .reduce(toPairs, []);

  const distance = (i, j) => getDistance(points[i], points[j]);

  const distances =
  [
    distance(0, 1),
    distance(0, 2),
    distance(0, 3),
    distance(1, 2),
    distance(1, 3),
    distance(2, 3)
  ].sort();

  console.log(isSquare(distances));
});
