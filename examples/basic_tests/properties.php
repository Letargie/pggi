<?php
namespace PGGI;
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
	$b->title = "Window Test";
	echo("{$b->title}\n");
	$b->hAlign = 3;
	echo("{$b->hAlign}\n");
	var_dump($b);
	$b->setDefaultSize(200, 200);
	$b->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_SHUTDOWN,"PGGI\\shutdown");
$a->on(SIGNAL_GAPPLICATION_STARTUP,"PGGI\\starting");
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();
