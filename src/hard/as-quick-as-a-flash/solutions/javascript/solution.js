#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function quickSort(values)
{
  let partitionCount = 0;

  function sort(head, tail)
  {
    if(head >= tail) return;

    const pivot = ((head, tail) =>
    {
      for(const pivot = values[head];;)
      {
        while(values[head] < pivot) ++head;
        while(values[tail] > pivot) --tail;

        if(head >= tail) return tail;

        const oldHeadValue = values[head];

        values[head] = values[tail];
        values[tail] = oldHeadValue;
      }
    })(head, tail);

    sort(head, pivot - 1);
    sort(pivot + 1, tail);

    ++partitionCount;
  }

  sort(0, values.length - 1);

  return partitionCount;
}

lineReader.on('line', (line) =>
{
  console.log(quickSort(line.split(' ').map(Number)));
});
