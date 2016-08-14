#!/usr/bin/env php
<?= join(PHP_EOL, array_map('decbin', file($argv[1]))), PHP_EOL;
