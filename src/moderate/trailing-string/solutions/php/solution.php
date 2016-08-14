#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  list($haystack, $needle) = $line;

  $position = strrpos($haystack, $needle,
    (strlen($haystack) - strlen($needle)));

  echo ($position === false ? 0 : 1), PHP_EOL;
}
