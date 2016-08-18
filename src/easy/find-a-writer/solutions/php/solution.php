#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl('|');

foreach($inputFile as $line)
{
  list($lhs, $rhs) = $line;

  $rhs = array_map(function($i) use($lhs)
  {
    return $lhs[$i - 1];
  }, array_filter(explode(' ', $rhs), 'strlen'));

  echo join('', $rhs), PHP_EOL;
}
