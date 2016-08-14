#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $values)
{
  list($x, $n) = $values;

  $multiple = $x >> log($n, 2);

  if($multiple * $n != $x) ++$multiple;

  echo ($n * $multiple), PHP_EOL;
}
