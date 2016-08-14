#!/usr/bin/env php
<?php

$scope = 12;
$width =  4;

for($y = 1; $y <= $scope; ++$y)
{
  for($x = 1; $x <= $scope; ++$x)
  {
    echo str_pad($x * $y, $width, ' ', STR_PAD_LEFT);
  }

  echo PHP_EOL;
}
