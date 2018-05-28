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
	$b = new GWindow($app);
	$b->title = "test empty window";
	$b->setDefaultSize(200, 200);
	$b->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_SHUTDOWN,"PGGI\\shutdown");
$a->on(SIGNAL_GAPPLICATION_STARTUP ,"PGGI\\starting");
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();
