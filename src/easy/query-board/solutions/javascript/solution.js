#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

class Board
{
  constructor(width)
  {
    this.width = width;
    this.cells = [];

    for(let i = 0; i < width; ++i)
    {
      this.cells.push(new Array(width).fill(0));
    }
  }

  queryRow(index)
  {
    return this.cells[index].reduce((a, b) => a + b);
  }

  queryCol(index)
  {
    let result = 0;

    for(let i = 0; i < this.width; ++i)
    {
      result += this.cells[i][index];
    }

    return result;
  }

  setRow(index, value)
  {
    this.cells[index].fill(value);
  }

  setCol(index, value)
  {
    for(let i = 0; i < this.width; ++i)
    {
      this.cells[i][index] = value;
    }
  }
}

let board = new Board(256);

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  switch(line[0])
  {
    case 'QueryRow': console.log(board.queryRow(+line[1])); break;
    case 'QueryCol': console.log(board.queryCol(+line[1])); break;

    case 'SetRow': board.setRow(+line[1], +line[2]); break;
    case 'SetCol': board.setCol(+line[1], +line[2]); break;
  }
});
