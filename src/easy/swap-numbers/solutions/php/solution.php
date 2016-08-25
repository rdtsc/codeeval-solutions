#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $words)
{
  echo join(' ', array_map(function($word)
  {
    list($i, $j) = [0, (strlen($word) - 1)];
    list($word[$i], $word[$j]) = [$word[$j], $word[$i]];

    return $word;
  }, $words)), PHP_EOL;
}
