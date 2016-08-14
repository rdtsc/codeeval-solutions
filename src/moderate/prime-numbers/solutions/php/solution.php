#!/usr/bin/env php
<?php

$queries = file($argv[1], FILE_IGNORE_NEW_LINES);

$maxQuery = max($queries);

$isPrime = array_fill(0, $maxQuery, true);

for($i = 2, $limit = ((int)sqrt($maxQuery) + 1); $i < $limit; ++$i)
{
  if($isPrime[$i])
  {
    for($j = ($i ** 2); $j < $maxQuery; $j += $i)
    {
      $isPrime[$j] = false;
    }
  }
}

foreach($queries as $query)
{
  $terms = [];

  for($i = 2; $i < $query; ++$i) if($isPrime[$i])
  {
    array_push($terms, $i);
  }

  echo join(',', $terms), PHP_EOL;
}
