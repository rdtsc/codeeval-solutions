#!/usr/bin/env php
<?= join(PHP_EOL, array_map(function($n)
{
  return (int)!($n & 1);
}, file($argv[1])));
