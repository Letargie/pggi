<?php
/*
  +-----------------------------------------------------------+
  | Copyright (c) 2017 Collet Valentin                        |
  +-----------------------------------------------------------+
  | This source file is subject to version the BDS license,   |
  | that is bundled with this package in the file LICENSE     |
  +-----------------------------------------------------------+
  | Author: Collet Valentin <valentin@famillecollet.com>      |
  +-----------------------------------------------------------+
*/

chdir(__DIR__);

include("Gasped/Gasped.class.php");
include("doc/PGGI.doc.php");

chdir(__DIR__);

$gasped = new Gasped();
echo($gasped->buildExtension("pggi", $PGGIDocumentationDocFile, "../doc/src"));
