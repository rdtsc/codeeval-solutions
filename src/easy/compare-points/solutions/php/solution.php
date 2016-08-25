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
  list($x0, $y0, $x1, $y1) = $line;

  $bearing = '';

  if($y0 !== $y1) $bearing .= ($y0 > $y1 ? 'S' : 'N');
  if($x0 !== $x1) $bearing .= ($x0 > $x1 ? 'W' : 'E');

  echo ($bearing ? $bearing : 'here'), PHP_EOL;
}
