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

  while($i > 0 && $values[$i - 1] >= $values[$i]) --$i;

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

function isSorted($values)
{
  $items = $values;

  rsort($items);

  return ($values === $items);
}

foreach($inputFile as $n)
{
  $n = str_split($n);

  if(isSorted($n)) array_unshift($n, '0');

  nextPermutation($n);

  echo join('', $n), PHP_EOL;
}
