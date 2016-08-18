#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  preg_match_all('/(.+?)\1/', $line, $matches);

  $matches = call_user_func_array('array_merge_recursive', $matches);

  array_walk($matches, function(&$match)
  {
    $match = strlen($match);
  });

  echo (empty($matches) ? strlen($line) : min($matches)), PHP_EOL;
}
