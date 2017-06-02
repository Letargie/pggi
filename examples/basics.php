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

function test($app){
	echo "test";
}

$a = new GApplication();
$a->onShutdown("shutdown");
$a->onStartup("starting");
$a->onActivate("activate");
echo("----------\n");
$a->run();
