#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

function nextPermutation(&$values)
{
  $width = count($values) - 1;

  $i = $j = $width;

  while($i > 0 && $values[$i - 1] > $values[$i]) --$i;

  if(!$i) return false;

  for(--$i; $values[$j] <= $values[$i]; --$j);

  $stash = $values[$i];

  $values[$i] = $values[$j];
  $values[$j] = $stash;

  for(++$i, $j = $width; $j > $i; ++$i, --$j)
  {
    $stash = $values[$i];
    $values[$i] = $values[$j];
    $values[$j] = $stash;
  }

  return true;
}

foreach($inputFile as $line)
{
  $line = str_split($line);

  sort($line);

  echo join('', $line);

  while(nextPermutation($line))
  {
    echo ',', join('', $line);
  }

  echo PHP_EOL;
}
