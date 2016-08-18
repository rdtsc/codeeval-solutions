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
  $line = array_reduce($line, function($a, $b)
  {
    return (strlen($a) >= strlen($b) ? $a : $b);
  });

  echo $line, PHP_EOL;
}
