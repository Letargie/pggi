<?php
namespace PGGI;
chdir(__DIR__);

function shutdown($app){
	echo("fermeture\n");
}

function starting(GApplication $app){
	echo "starting\n";
}

function activate($app){
	echo "activate\n";
	$w = new GWindow($app, "example button");
	$w->setDefaultSize(200, 200);
	$button = new GButton("hey !");
	$w->add($button);
	$w->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_SHUTDOWN,"PGGI\\shutdown");
$a->on(SIGNAL_GAPPLICATION_STARTUP,"PGGI\\starting");
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();
