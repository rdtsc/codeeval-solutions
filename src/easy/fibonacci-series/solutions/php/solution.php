#!/usr/bin/env php
<?php

$queries = file($argv[1], FILE_IGNORE_NEW_LINES);

$maxQuery = max($queries);

$terms = [0, 1];

for($i = 2; $i <= $maxQuery; ++$i)
{
  array_push($terms, ($terms[$i - 1] + $terms[$i - 2]));
}

foreach($queries as $query)
{
  echo $terms[$query], PHP_EOL;
}
