#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$keypad =
[
  ['0'],
  ['1'],
  ['a', 'b', 'c',],
  ['d', 'e', 'f',],
  ['g', 'h', 'i',],
  ['j', 'k', 'l',],
  ['m', 'n', 'o',],
  ['p', 'q', 'r', 's'],
  ['t', 'u', 'v',],
  ['w', 'x', 'y', 'z']
];

foreach($inputFile as $line)
{
  $results = [];

  foreach($keypad[$line[0]] as $c0)
  foreach($keypad[$line[1]] as $c1)
  foreach($keypad[$line[2]] as $c2)
  foreach($keypad[$line[3]] as $c3)
  foreach($keypad[$line[4]] as $c4)
  foreach($keypad[$line[5]] as $c5)
  foreach($keypad[$line[6]] as $c6)
  {
    array_push($results, "$c0$c1$c2$c3$c4$c5$c6");
  }

  echo join(',', $results), PHP_EOL;
}
