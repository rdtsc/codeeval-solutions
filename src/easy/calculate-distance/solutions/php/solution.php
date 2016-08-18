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

  list($x0, $y0, $x1, $y1) = $line;

  echo hypot(($x1 - $x0), ($y1 - $y0)), PHP_EOL;
}
