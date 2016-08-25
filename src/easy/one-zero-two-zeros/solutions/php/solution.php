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

  $tally = 0;

  for($i = 1; $i <= $rhs; ++$i)
  {
    $tally += (substr_count(decbin($i), '0') == $lhs);
  }

  echo $tally, PHP_EOL;
}
