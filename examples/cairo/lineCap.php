<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);


$context->lineWidth = 30.0;
$context->lineCap = Context::LINE_CAP_BUTT; // default one
$context->moveTo(64, 50)
        ->lineTo(64, 200)
        ->stroke()
        ->lineCap = Context::LINE_CAP_ROUND;
$context->moveTo(128, 50)
        ->lineTo(128, 200)
        ->stroke()
        ->lineCap = Context::LINE_CAP_SQUARE;
$context->moveTo(192, 50)
        ->lineTo(192, 200)
        ->stroke()
        ->setSource(1, 0.2, 0.2)
        ->lineWidth = 2.56;
$context->moveTo(64, 50)
        ->lineTo(64, 200)
        ->moveTo(128, 50)
        ->lineTo(128, 200)
        ->moveTo(192, 50)
        ->lineTo(192, 200)
        ->stroke();

$surface->writeToPNG("lineCap.png");

