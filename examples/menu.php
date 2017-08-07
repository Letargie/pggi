<?php

chdir(__DIR__);

function shutdown($app){
	echo("fermeture\n");
}

function starting(GApplication $app){
	echo "starting\n";
}

function activate($app){
	echo "activate\n";
	
	$box = new GBox(PGGI_ORIENTATION_VERTICAL, 2);
	$bar = new GMenuBar();
	$itemA = new GMenuItem("Test1");
	$itemB = new GMenuItem("Test2");
	$itemC = new GMenuItem("Test3");
	$gmenuA = new GMenu();
	$gmenuB = new GMenu();
	$gmenuC = new GMenu();
	$itemAA = new GMenuItem("AA");
	$itemAB = new GMenuItem("AB");
	$itemBA = new GMenuItem("BA");
	$itemCA = new GMenuItem("CA");
	$itemCB = new GMenuItem("CB");
	$itemCC = new GMenuItem("CC");

	$gmenuA->add($itemAA);
	$gmenuA->add($itemAB);
	$gmenuB->add($itemBA);
	$gmenuC->add($itemCA);
	$gmenuC->add($itemCB);
	$gmenuC->add($itemCC);
	
	$bar->add($itemA);
	$bar->add($itemB);
	$bar->add($itemC);
	
	$itemA->submenu = $gmenuA;
	$itemB->submenu = $gmenuB;
	$itemC->submenu = $gmenuC;
	
	$box->add($bar);
	$box->add(new GLabel("Lorem Ipsum..."));
	$w = new GWindow($app, "example menu");
	$w->setDefaultSize(200, 200);

	$w->add($box);
	$w->showAll();
}

$a = new GApplication();
$a->on(PGGI_SIGNAL_GAPPLICATION_SHUTDOWN,"shutdown");
$a->on(PGGI_SIGNAL_GAPPLICATION_STARTUP,"starting");
$a->on(PGGI_SIGNAL_GAPPLICATION_ACTIVATE,"activate");
$a->run();
