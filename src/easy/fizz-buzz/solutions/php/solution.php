#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $line)
{
  list($x, $y, $n) = $line;

  for($i = 1; $i <= $n; ++$i)
  {
    $result = str_repeat('F', !($i % $x)) .
              str_repeat('B', !($i % $y));

    echo ($result === '' ? $i : $result), ' ';
  }

  echo PHP_EOL;
}
