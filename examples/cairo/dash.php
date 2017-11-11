<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$dashes = array(50, 10, 10, 10);
$offset = -50.0;

$context->setDash($dashes, $offset)
        ->lineWidth = 10.0;
$context->moveTo(128, 25.6)
        ->lineTo(230.4, 230.4)
        ->relLineTo(-102.4, 0)
        ->curveTo(51.2, 230.4, 51.2, 128, 128, 128)
        ->stroke();

$surface->writeToPNG("dash.png");
