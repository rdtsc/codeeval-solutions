#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $payload = json_decode($line, true);
  $payload = call_user_func_array('array_merge_recursive', $payload);

  $result = 0;

  foreach($payload['items'] as $item)
  {
    if(isset($item['label'])) $result += $item['id'];
  }

  echo $result, PHP_EOL;
}
