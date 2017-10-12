<?php

namespace PGGI\Cairo;

chdir(__DIR__);


$xc = 128.0;
$yc = 128.0;
$radius = 100.0;
$angle1 = 45.0  * (pi()/180.0);  /* angles are specified */
$angle2 = 180.0 * (pi()/180.0);  /* in radians           */

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->lineWidth = 10;
$context->arcNegative($xc, $yc, $radius, $angle1, $angle2)
        ->stroke();

		/*Drawing helping lines*/
$context->setSourceRGBA(1, 0.2, 0.2, 0.6)
        ->lineWidth = 6;

$context->arc($xc, $yc, 10, 0, 2*pi())
        ->fill();

$context->arc($xc, $yc, $radius, $angle1, $angle1)
        ->lineTo($xc, $yc)
        ->arc($xc, $yc, $radius, $angle2, $angle2)
        ->lineTo($xc, $yc)
        ->stroke();

$surface->writeToPNG("arcNegative.png");


