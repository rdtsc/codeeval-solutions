#!/usr/bin/env php
<?= ['BigEndian', 'LittleEndian'][unpack('S', "\x1\x0")[1] === 1], PHP_EOL;
