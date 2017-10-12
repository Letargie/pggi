<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$x=25.6  ; $y=128.0 ;
$x1=102.4; $y1=230.4;
$x2=153.6; $y2=25.6 ;
$x3=230.4; $y3=128.0;

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->moveTo($x, $y)
        ->curveTo($x1, $y1, $x2, $y2, $x3, $y3)
        ->lineWidth = 10.0;

$context->stroke()
        ->setSource(1, 0.2, 0.2, 0.6)
        ->lineWidth = 6.0;
$context->moveTo($x, $y)
        ->lineTo($x1, $y1)
        ->moveTo($x2, $y2)
        ->lineTo($x3, $y3)
        ->stroke();

$surface->writeToPNG("curveTo.png");
