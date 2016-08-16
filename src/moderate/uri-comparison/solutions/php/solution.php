#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(';');

foreach($inputFile as $line)
{
  list($lhs, $rhs) = array_map(function($url)
  {
    $url = parse_url(urldecode($url));

    foreach(['scheme', 'host'] as $component)
    {
      $url[$component] = strtolower($url[$component]);
    }

    if(!array_key_exists('port', $url)) $url['port'] = 80;

    return $url;
  }, $line);

  echo ($lhs == $rhs ? 'True' : 'False'), PHP_EOL;
}
