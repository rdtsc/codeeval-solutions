#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl('|');

foreach($inputFile as $values)
{
  array_walk($values, function(&$n)
  {
    $n = intval($n, 2);

    $n ^= ($n >> 1);
    $n ^= ($n >> 2);
    $n ^= ($n >> 4);
  });

  echo join(' | ', $values), PHP_EOL;
}
