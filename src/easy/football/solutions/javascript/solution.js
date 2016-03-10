#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  let results = {};

  line.forEach((teams, country) =>
  {
    ++country;

    teams.split(' ').forEach((team) =>
    {
      if(!results[team]) results[team] = [country];

      else results[team].push(country);
    });
  });

  line = [];

  for(const team in results)
  {
    const countries = results[team].join(',');

    line.push(team + ':' + countries);
  }

  line.push(null);

  console.log(line.join('; '));
});
