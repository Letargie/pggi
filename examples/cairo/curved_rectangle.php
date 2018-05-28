<?php

namespace PGGI\Cairo;

chdir(__DIR__);


function curvedRectangle($context, $x0, $y0, $width, $height, $radius){
	$x1 = $x0 + $width;
	$y1 = $y0 + $height;

	if(!$width || !$height)
		return;
	if($width/2 < $radius){
		if($height/2 < $radius){
			$context->moveTo($x0, ($y0+$y1)/2)
			        ->curveTo($x0, $y0, $x0, $y0, ($x0 + $x1)/2, $y0)
			        ->curveTo($x1, $y0, $x1, $y0, $x1, ($y0 + $y1)/2)
			        ->curveTo($x1, $y1, $x1, $y1, ($x1 + $x0)/2, $y1)
			        ->curveTo($x0, $y1, $x0, $y1, $x0, ($y0 + $y1)/2);
		}else{
			$context->moveTo($x0, $y0 + $radius)
			        ->curveTo($x0, $y0, $x0, $y0, ($x0 + $x1)/2, $y0)
			        ->curveTo($x1, $y0, $x1, $y0, $x1, $y0 + $radius)
			        ->lineTo($x1, $y1 - $radius)
			        ->curveTo($x1, $y1, $x1, $y1, ($x1 + $x0)/2, $y1)
			        ->curveTo($x0, $y1, $x0, $y1, $x0, $y1 - $radius);
		}
	}else{
		if($height/2 < $radius){
			$context->moveTo($x0, ($y0+$y1)/2)
			        ->curveTo($x0, $y0, $x0, $y0, $x0 + $radius, $y0)
			        ->lineTo($x1 - $radius, $y0)
			        ->curveTo($x1, $y0, $x1, $y0, $x1, ($y0 + $y1)/2)
			        ->curveTo($x1, $y1, $x1, $y1, $x1 - $radius, $y1)
			        ->curveTo($x0, $y1, $x0, $y1, $x0, ($y0 + $y1)/2);
		}else{
			$context->moveTo($x0, $y0 + $radius)
			        ->curveTo($x0, $y0, $x0, $y0, $x0 + $radius, $y0)
			        ->lineTo($x1 - $radius, $y0)
			        ->curveTo($x1, $y0, $x1, $y0, $x1, $y0 + $radius)
			        ->curveTo($x1, $y1, $x1, $y1, $x1 - $radius, $y1)
			        ->curveTo($x0, $y1, $x0, $y1, $x0, $y1 - $radius);
		}
	}
	$context->closePath()
	    ->setSource(0.5, 0.5, 1)
	    ->fillPreserve()
	    ->setSource(0.5, 0, 0, 0.5)
	    ->lineWidth = 10.0;
	$context->stroke();
	return $context;
}

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

curvedRectangle($context, 25.6, 25.6, 204.8, 204.8, 102.4);

$surface->writeToPNG("curved_rectangle.png");




