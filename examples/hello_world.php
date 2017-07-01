<?php

function activate($app){
	$label = new GLabel("Hello World !");
	$win = new GWindow($app);
	$win->setTitle("Window Test");
	$win->setDefaultSize(200, 200);
	$win->add($label);
	$win->showAll();
}

$a = new GApplication();
$a->on(PGGI_SIGNAL_GAPPLICATION_ACTIVATE, "activate");
$a->run();
