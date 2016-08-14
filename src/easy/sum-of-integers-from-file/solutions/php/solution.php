#!/usr/bin/env php
<?= array_sum(file($argv[1], FILE_IGNORE_NEW_LINES)), PHP_EOL;
