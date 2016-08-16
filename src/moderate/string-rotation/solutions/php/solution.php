#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  list($lhs, $rhs) = $line;

  echo (strpos("$lhs$lhs", $rhs) !== false ? 'True' : 'False'), PHP_EOL;
}
