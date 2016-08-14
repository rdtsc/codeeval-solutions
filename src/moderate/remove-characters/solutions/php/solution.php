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

  $needle = trim($needle);

  echo preg_replace("/[$needle]/", '', $haystack), PHP_EOL;
}
