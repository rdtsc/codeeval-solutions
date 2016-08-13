#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(';');

$mapping =
[
  'zero' => 0, 'one' => 1, 'two'   => 2, 'three' => 3, 'four' => 4,
  'five' => 5, 'six' => 6, 'seven' => 7, 'eight' => 8, 'nine' => 9
];

foreach($inputFile as $line)
{
  array_walk($line, function(&$value) use($mapping)
  {
    $value = $mapping[$value];
  });

  echo join('', $line), PHP_EOL;
}
