#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

function stableSort(&$array, $callback)
{
  $i = 0;

  foreach($array as &$entry)
  {
    $entry = ['value' => $entry, 'index' => ++$i];
  }

  $result = usort($array, function($lhs, $rhs) use($callback)
  {
    $result = call_user_func($callback, $lhs['value'], $rhs['value']);

    return ($result === 0 ? ($lhs['index'] - $rhs['index']) : $result);
  });

  foreach($array as &$item) $item = $item['value'];

  return $result;
}

foreach($inputFile as $line)
{
  preg_match_all('/(.+)(?=.*\1)/', $line, $matches);

  $matches = call_user_func_array('array_merge', $matches);

  $matches = array_filter($matches, function($match)
  {
    return ($match !== ' ');
  });

  stableSort($matches, function($lhs, $rhs)
  {
    return (strlen($rhs) - strlen($lhs));
  });

  echo (empty($matches) ? 'NONE' : $matches[0]), PHP_EOL;
}
