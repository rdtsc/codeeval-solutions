#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $values)
{
  list($n, $x, $y) = $values;

  $n = decbin($n);
  $w = strlen($n);

  echo ($n[$w - $x] === $n[$w - $y] ? 'true' : 'false'), PHP_EOL;
}
