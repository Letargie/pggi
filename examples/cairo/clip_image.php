<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->arc(128, 128, 78.8, 0, 2 * pi())
        ->clip()
        ->newPath(); /*path not consumed by clip()*/
$png = new ImageSurface("data/romedalen.png");
$context->scale(256 / $png->getWidth(), 256 / $png->getHeight())
        ->setSource($png, 0, 0)
        ->paint();

$surface->writeToPNG("clip_image.png");
