<?php

function shutdown($app){
	echo("fermeture\n");
}

function starting(GApplication $app){
	echo "starting\n";
}

function activate($app){
	echo "activate\n";
	echo("-----\n");
	$b = new GWindow($app);
	echo("-----\n");
	
	var_dump($b);
	//$b->setTitle("Window Test");
	$b->title = "Window Test";
	echo("{$b->title}\n");
	$b->hAlign = 3;
	echo("{$b->hAlign}\n");
	var_dump($b);
	$b->setDefaultSize(200, 200);
	$b->showAll();
}

$a = new GApplication();
$a->on(PGGI_SIGNAL_GAPPLICATION_SHUTDOWN,"shutdown");
$a->on(PGGI_SIGNAL_GAPPLICATION_STARTUP,"starting");
$a->on(PGGI_SIGNAL_GAPPLICATION_ACTIVATE,"activate");
$a->run();
