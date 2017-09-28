<?php
namespace PGGI;
/*
function activate($app, $data){
	$label = new GLabel("Hello World !");
	$win = new GWindow($app, "Hello World !");
	$win->setDefaultSize(200, 200);
	$win->add($label);
	$win->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, "PGGI\\activate");
$a->run();*/

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, function($app, $data){
	$label = new GLabel("Hello World !");
	$win = new GWindow($app, "Hello World !");
	$win->setDefaultSize(200, 200);
	$win->add($label);
	$win->showAll();
});
$a->run();
