<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->moveTo(128, 25.6)
        ->lineTo(230.4, 230.4)
        ->relLineTo(-102.4, 0)
        ->curveTo(51.2, 230.4, 51.2, 128, 128, 128)
        ->closePath()
        ->moveTo(64, 25.6)
        ->relLineTo(51.2, 51.2)
        ->relLineTo(-51.2, 51.2)
        ->relLineTo(-51.2, -51.2)
        ->closePath()
        ->lineWidth = 10.0;
$context->setSource(0, 0, 1)
        ->fillPreserve()
        ->setSource(0, 0, 0)
        ->stroke();

$surface->writeToPNG("fill_and_stroke2.png");
