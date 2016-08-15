#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  list($x1, $y1, $x2, $y2,
       $x3, $y3, $x4, $y4) = $line;

  $x = ($x1 > $x4 || $x2 < $x3);
  $y = ($y1 < $y4 || $y2 > $y3);

  echo ($x || $y ? 'False' : 'True'), PHP_EOL;
}
