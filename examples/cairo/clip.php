<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->arc(128, 128, 78.8, 0, 2 * pi())
        ->clip();

/*current path is not consumed by clip()*/
$context->newPath()
        ->rectangle(0, 0, 256, 256)
        ->fill()
		->setSourceRGBA(0, 1, 0)
        ->moveTo(0, 0)
        ->lineTo(256, 256)
        ->moveTo(256, 0)
        ->lineTo(0, 256)
        ->lineWidth = 10.0;

$context->stroke();

$surface->writeToPNG("clip.png");


