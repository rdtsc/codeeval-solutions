#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(';');

foreach($inputFile as $line)
{
  list($lhs, $rhs) = $line;

  $lhs = explode(',', $lhs);
  $rhs = explode(',', $rhs);

  echo join(',', array_intersect($lhs, $rhs)), PHP_EOL;
}
