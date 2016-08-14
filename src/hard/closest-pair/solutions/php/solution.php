#!/usr/bin/env php
<?php

$cases = preg_split('/^\d+$/m', file_get_contents($argv[1]));

function toLines($line)
{
  return explode(PHP_EOL, $line);
}

function toPoints($values)
{
  $values = array_filter($values, function($value)
  {
    return ($value !== '');
  });

  $values = array_map(function($value)
  {
    list($x, $y) = explode(' ', $value);

    return ['x' => (int)$x, 'y' => (int)$y];
  }, $values);

  return array_values($values);
}

function getClosestPairDistance($points)
{
  $pointCount  = count($points);
  $minDistance = PHP_INT_MAX;

  for($i =     0   ; $i < $pointCount; ++$i)
  for($j = ($i + 1); $j < $pointCount; ++$j)
  {
    $deltaX = $points[$i]['x'] - $points[$j]['x'];
    $deltaY = $points[$i]['y'] - $points[$j]['y'];

    $distance = ($deltaX ** 2) + ($deltaY ** 2);

    if($minDistance > $distance) $minDistance = $distance;
  }

  return sqrt($minDistance);
}

$cases = array_map('toLines',  $cases);
$cases = array_map('toPoints', $cases);

$cases = array_filter($cases, function($case)
{
  return !empty($case);
});

foreach($cases as $points)
{
  $distance = getClosestPairDistance($points);

  $result = 'INFINITY';

  if($distance < 1e5) $result = number_format($distance, 4, '.', '');

  echo $result, PHP_EOL;
}
