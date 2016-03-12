#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(';', ',')
             .split(',')
             .map(pair => pair.split(' ').map(Number));

  const vertexCount = line[0][0],
        edgeCount   = line[0][1];

  line.shift();

  let edges = Array(vertexCount).fill().map(_ => Array(vertexCount).fill(0));

  line.forEach((edge) =>
  {
    edges[edge[0]][edge[1]] = 1;
    edges[edge[1]][edge[0]] = 1;
  });

  let scratchA = edges.map(row => row.slice()),
      scratchB = edges.map(row => row.slice());

  for(let i = 0; i < 2; ++i)
  {
    for(let y = 0; y < vertexCount; ++y)
    for(let x = 0; x < vertexCount; ++x)
    {
      let cell = 0;

      for(let j = 0; j < vertexCount; ++j)
      {
        cell += scratchA[y][j] * edges[j][x];
      }

      scratchB[y][x] = cell;
    }

    scratchA = scratchB.map(row => row.slice());
  }

  let trace = 0;

  for(let i = 0; i < vertexCount; ++i)
  {
    trace += scratchA[i][i];
  }

  console.log(Math.trunc(trace / 6));
});
