<?php
namespace PGGI;

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, function ($app, $data){
	$label = new GLabel("Hello World !");
	$win = new GWindow($app, "Hello World !");
	$win->setDefaultSize(200, 200);
	$win->add($label);
	$win->showAll();
});
$a->run();
