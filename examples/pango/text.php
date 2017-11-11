<?php

namespace PGGI\Pango;
use \PGGI\Cairo\Context as CContext;
use \PGGI\Cairo\ImageSurface;

chdir(__DIR__);

$radius = 150;
$nbWords =  10;
$font = "Sans Bold 27";

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 2 * $radius, 2 * $radius);
$context = new CContext($surface);
$context->setSource(1., 1., 1.)
        ->paint();

$context->translate($radius, $radius);

$layout = new Layout($context);
$layout->text = 'Text';

$description = new FontDescription($font);
$layout->fontDescription = $description;


for($i = 0; $i < $nbWords; ++$i){
	$width = 0;
	$height = 0;
	$angle = (360 * $i) / $nbWords;

	$context->save();
	$red = (1 + cos(($angle - 60) * pi() / 180)) / 2;
	$context->setSource($red, 0, 1 - $red)
	        ->rotate($angle * pi() / 180);

	$context->updateLayout($layout);

	$layout->getSize($width, $height);
	$context->moveTo(-((double)$width / SCALE) / 2, - $radius);
	$context->showLayout($layout);

	$context->restore();
}


$surface->writeToPNG("text.png");


