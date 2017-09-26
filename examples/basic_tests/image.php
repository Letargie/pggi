<?php
namespace PGGI;
chdir(__DIR__);

function activate($app){
	$image = new GImage("resources/php.png");
	$win = new GWindow($app, "Image Display");
	$win->setDefaultSize(400, 400);
	$win->add($image);
	$win->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, "PGGI\\activate");
$a->run();
