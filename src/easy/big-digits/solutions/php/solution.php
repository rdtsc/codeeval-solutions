#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

const SEGMENTS =
[
  ['-**--', '*--*-', '*--*-', '*--*-', '-**--', '-----'],
  ['--*--', '-**--', '--*--', '--*--', '-***-', '-----'],
  ['***--', '---*-', '-**--', '*----', '****-', '-----'],
  ['***--', '---*-', '-**--', '---*-', '***--', '-----'],
  ['-*---', '*--*-', '****-', '---*-', '---*-', '-----'],
  ['****-', '*----', '***--', '---*-', '***--', '-----'],
  ['-**--', '*----', '***--', '*--*-', '-**--', '-----'],
  ['****-', '---*-', '--*--', '-*---', '-*---', '-----'],
  ['-**--', '*--*-', '-**--', '*--*-', '-**--', '-----'],
  ['-**--', '*--*-', '-***-', '---*-', '-**--', '-----']
];

foreach($inputFile as $line)
{

  $digits = array_values(array_filter(str_split($line), function($chunk)
  {
    return is_numeric($chunk);
  }));

  $digitCount   = count($digits);
  $segmentWidth = count(SEGMENTS[0]);

  for($i = 0; $i < $segmentWidth; ++$i)
  {
    for($j = 0; $j < $digitCount; ++$j)
    {
      echo SEGMENTS[$digits[$j]][$i];
    }

    echo PHP_EOL;
  }
}
