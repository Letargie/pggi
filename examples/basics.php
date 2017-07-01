<?php

function shutdown($app){
	echo("fermeture\n");
}

function starting(GApplication $app){
	echo "starting\n";
}

function activate($app){
	echo "activate\n";
	$b = new GWindow($app);
	$b->setTitle("Window Test");
	$b->setDefaultSize(200, 200);
	$b->showAll();
}

$a = new GApplication();
$a->on(PGGI_SIGNAL_GAPPLICATION_SHUTDOWN,"shutdown");
$a->on(PGGI_SIGNAL_GAPPLICATION_STARTUP,"starting");
$a->on(PGGI_SIGNAL_GAPPLICATION_ACTIVATE,"activate");
$a->run();
