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
  list($lhs, $rhs) = $line;

  $lhs = str_split($lhs);
  $rhs = preg_split('/([-+])/', $rhs, null, PREG_SPLIT_DELIM_CAPTURE);

  $op = $rhs[1];
  $opIndex = strlen($rhs[0]);

  $x = join('', array_slice($lhs, 0, $opIndex));
  $y = join('', array_slice($lhs, $opIndex));

  echo (($op === '+') ? ($x + $y) : ($x - $y)), PHP_EOL;
}
