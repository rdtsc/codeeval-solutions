#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

function getVariantCount($message)
{
  $chunkWidth = (strlen($message) >> 1);

  list($lhs, $rhs) = str_split($message, $chunkWidth);

  $asteriskCount = 0;

  for($i = 0; $i < $chunkWidth; ++$i)
  {
    $a = $lhs[$i];
    $b = $rhs[$i];

    if("$a$b" === '**') ++$asteriskCount;

    else if(($a !== $b) && ($a !== '*') && ($b !== '*'))
    {
      return 0;
    }
  }

  return (2 ** $asteriskCount);
}

foreach($inputFile as $message)
{
  echo getVariantCount($message), PHP_EOL;
}
