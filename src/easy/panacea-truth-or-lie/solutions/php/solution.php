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

  $lhs = array_filter(explode(' ', $lhs), 'strlen');
  $rhs = array_filter(explode(' ', $rhs), 'strlen');

  $lhs = array_map(function($n) {return intval($n, 16);}, $lhs);
  $rhs = array_map(function($n) {return intval($n,  2);}, $rhs);

  $lhs = array_sum($lhs);
  $rhs = array_sum($rhs);

  echo ['False', 'True'][$lhs <= $rhs], PHP_EOL;
}
