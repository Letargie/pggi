<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$png = new ImageSurface("data/romedalen.png");
$w = $png->getWidth();
$h = $png->getHeight();

$context->translate(128, 128)
        ->rotate(45 * pi() / 180)
        ->scale(256/$w, 256/$h)
        ->translate(-0.5*$w, -0.5*$h)
        ->setSource($png, 0, 0)
        ->paint();

$surface->writeToPNG("image.png");
