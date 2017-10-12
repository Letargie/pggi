<?php
namespace PGGI;

function draw_callback($widget, $data, $cairo_context){
	$context = $widget->getStyleContext();
	$w = $widget->getAllocatedWidth();
	$h = $widget->getAllocatedHeight();
	$context->renderBackground($cairo_context, 0, 0, $w, $h);
	
	$color = $context->getColor($context->getState());

	$cairo_context->arc($w/2, $h/2, min($w, $h)/2, 0, 2 * pi())->setColor($color)->fill();
	//$cairo_context->arc($w/2, $h/2, min($w, $h)/2, 0, 2 * pi())->setSource($color)->fill();
	//return false;
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, function ($app, $data){
	$area = new GDrawingArea();
		$area->on(SIGNAL_GWIDGET_DRAW, "PGGI\\draw_callback");
	$win = new GWindow($app, "Hello World !");
	$win->setDefaultSize(100, 100);
	$win->add($area);
	$win->showAll();
});
$a->run();
