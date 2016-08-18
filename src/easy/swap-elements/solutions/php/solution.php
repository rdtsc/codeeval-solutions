#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(':');

function normalize($string, $delimiter = ' ')
{
  return array_filter(explode($delimiter, $string), function($item)
  {
    return strlen($item);
  });
}

function swap(&$lhs, &$rhs)
{
  list($lhs, $rhs) = [$rhs, $lhs];
}

foreach($inputFile as $line)
{
  list($lhs, $rhs) = $line;

  $lhs = normalize($lhs);
  $rhs = normalize(preg_replace('/\D/', ' ', $rhs));
  $rhs = array_chunk($rhs, 2);

  foreach($rhs as $index)
  {
    swap($lhs[$index[0]], $lhs[$index[1]]);
  }

  echo join(' ', $lhs), PHP_EOL;
}
