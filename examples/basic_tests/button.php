<?php
namespace PGGI;
chdir(__DIR__);

function activate($app, $data){
	echo "activate\n";
	$w = new GWindow($app, "example button");
	$w->setDefaultSize(200, 200);
	$button = new GButton("hey !");
	$w->add($button);
	$button->on(SIGNAL_GBUTTON_CLICKED, function($button, $parent){
var_dump($parent);
		$dialog = new GMessageDialog($parent, GDialog::FLAG_MODAL, GMessageDialog::MESSAGE_INFO, GMessageDialog::BUTTONS_OK, 'You\'ve clicked on the button !');
		$dialog->run();
	}, $w);
	$w->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();
