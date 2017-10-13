<?php

namespace PGGI\Cairo;

chdir(__DIR__);

function roundedRectangle($context, $x, $y, $width, $height, $aspect){
	if($aspect == 0)
		return;
	$corner_radius = $height / 10;
	$radius = $corner_radius / $aspect;
	$degrees = pi() / 180;

	$context->newSubPath()
	        ->arc($x + $width - $radius, $y + $radius          , $radius, -90 * $degrees, 0             )
	        ->arc($x + $width - $radius, $y + $height - $radius, $radius, 0             , 90  * $degrees )
	        ->arc($x + $radius         , $y + $height - $radius, $radius, 90  * $degrees, 180 * $degrees)
	        ->arc($x + $radius         , $y + $radius          , $radius, 180 * $degrees, 270 * $degrees)
	        ->closePath()
	        ->setSource(0.5, 0.5, 1)
	        ->fillPreserve()
	        ->setSource(0.5, 0, 0, 0.5)
	        ->lineWidth = 10.0;
	$context->stroke();
	return $context;
}

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

roundedRectangle($context, 25.6, 25.6, 204.8, 204.8, 1);

$surface->writeToPNG("rounded_rectangle.png");

    

