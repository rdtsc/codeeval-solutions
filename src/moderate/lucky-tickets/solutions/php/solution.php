#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

const TERMS =
[
  /*   2 */ '10',
  /*   4 */ '670',
  /*   6 */ '55252',
  /*   8 */ '4816030',
  /*  10 */ '432457640',
  /*  12 */ '39581170420',
  /*  14 */ '3671331273480',
  /*  16 */ '343900019857310',
  /*  18 */ '32458256583753952',
  /*  20 */ '3081918923741896840',
  /*  22 */ '294056694657804068000',
  /*  24 */ '28170312778225750242100',
  /*  26 */ '2707859169387181467852100',
  /*  28 */ '261046730157780861858821136',
  /*  30 */ '25228791861003454642059261392',
  /*  32 */ '2443553412892220489195278947230',
  /*  34 */ '237126779700111728623210793896700',
  /*  36 */ '23050391247812238203687824747157800',
  /*  38 */ '2244066255357188250744344225634235600',
  /*  40 */ '218768894829904122626725603838896148680',
  /*  42 */ '21353575263502884630388273714197145182600',
  /*  44 */ '2086610157206763614866736016835099941514800',
  /*  46 */ '204105559167234718098196864419783302549632800',
  /*  48 */ '19983598742087310057357815311872236715171970100',
  /*  50 */ '1958235988893910037740658552689739094876481545140',
  /*  52 */ '192043570409723719873997281482556737440155994069900',
  /*  54 */ '18847430845804472530652413091697822872821444349147800',
  /*  56 */ '1850969282556229733639327066252630957954651870505893968',
  /*  58 */ '181894832005057034383999852869437328659060112416355888580',
  /*  60 */ '17885336470694891279287104747395798264488027368907029555640',
  /*  62 */ '1759595312376617572854059903364131713003363235732517504574240',
  /*  64 */ '173201762205643166279968008940412331699470273735812696599774110',
  /*  66 */ '17056971894934558688846441987433052988620303403449436294441959820',
  /*  68 */ '1680541301718359604648576749737521003907285387827431001567574176596',
  /*  70 */ '165646654319976723701992406615067655308208865219762876621727020294840',
  /*  72 */ '16333976304699214718657221877442624229398621392350520803192093730809000',
  /*  74 */ '1611266420446186900661455564918186756941611539777885816296938086322793400',
  /*  76 */ '159001106265949634483960692567311589105760040472435056876279269251280262400',
  /*  78 */ '15695752713441211517384309707116838588162950015054857902560999394638257318080',
  /*  80 */ '1549907697862929680336583303564541550920867807532198717258856653744296207074760',
  /*  82 */ '153096138752277110214836722220881087457063409672964274344894548977869525514285400',
  /*  84 */ '15126933076887535241547078349055723957328538056942697111947324195746641221987741240',
  /*  86 */ '1495063936394820145978173084952651226318895610169643711778698526582132897696194675360',
  /*  88 */ '147803690249353793831835578756919945270648840422121798343465830237679031761117155521520',
  /*  90 */ '14615786698841561369944959191566195353879505580317403926959872129388565881505390562968052',
  /*  92 */ '1445658259577075287740895176423089081604032456789033769154490620014797374846744269788548840',
  /*  94 */ '143024700435101183997266162498015519369139255434050832382762644794442460391496029666811987520',
  /*  96 */ '14153183315801667166858772596125134421835467308472191612483110921724154905563283000857391746100',
  /*  98 */ '1400847570487018446424152137399824237820256780329223966658770240584725100387355667521375484520200',
  /* 100 */ '138681178063913146486663255108385891670476531416644888545033078503482282975641730091720919340564340'
];

foreach($inputFile as $n)
{
  echo (TERMS[($n >> 1) - 1]), PHP_EOL;
}
