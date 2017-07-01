<?php

chdir(__DIR__);

function activate($app){
	$image = new GImage("resources/php.png");
	$win = new GWindow($app);
	$win->setTitle("Image Display");
	$win->setDefaultSize(400, 400);
	$win->add($image);
	$win->showAll();
}

$a = new GApplication();
$a->on(PGGI_SIGNAL_GAPPLICATION_ACTIVATE, "activate");
$a->run();
