<?php

namespace PGGI;

function activate($app){
	echo "activate\n";
	$b = new GWindow($app);
	$b->title = "test window with fixed elements";
	$b->setDefaultSize(500, 500);

	$fixed = new GFixed();
	$b->add($fixed);

	$firstLabel = new GLabel("First label");
	$fixed->put($firstLabel, 0, 0);
	
	$secondaryLabel = new GLabel("Secondary label");
	$fixed->put($secondaryLabel, 40, 40);

	$thirdLabel = new GLabel("Third label");
	$fixed->put($thirdLabel, 460, 460);
	
	$b->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();
