#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = preg_replace('/[^\d\s.-]/', '', $line);
  $line = explode(' ', $line);
  $line = array_filter($line, 'strlen');
  $line = array_values($line);

  list($cx, $cy, $r, $px, $py) = $line;

  $distance = hypot(($px - $cx), ($py - $cy));

  echo ($distance < $r ? 'true' : 'false'), PHP_EOL;
}
