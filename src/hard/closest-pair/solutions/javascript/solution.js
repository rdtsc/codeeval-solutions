#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

const toLines = (line) => line.split('\n');

function toPoints(values)
{
  values = values.filter(value => value.length)
                 .map(chunk => chunk.split(' ').map(Number));

  return values.map(point => ({x: point[0], y: point[1]}));
}

function getClosestPairDistance(points)
{
  const pointCount = points.length;

  let minDistance = Number.MAX_VALUE;

  for(let i =   0  ; i < pointCount; ++i)
  for(let j = i + 1; j < pointCount; ++j)
  {
    const deltaX = points[i].x - points[j].x,
          deltaY = points[i].y - points[j].y;

    const distance = Math.pow(deltaX, 2) + Math.pow(deltaY, 2);

    if(minDistance > distance) minDistance = distance;
  }

  return Math.sqrt(minDistance);
}

const cases = slurp().split(/^\d+$/m)
                     .map(toLines)
                     .map(toPoints)
                     .filter(set => set.length);

cases.forEach((points) =>
{
  const distance = getClosestPairDistance(points);

  if(distance >= 1e5) console.log('INFINITY');

  else console.log(distance.toFixed(4));
});
