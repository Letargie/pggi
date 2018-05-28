<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);


$context->moveTo(50, 75)
        ->lineTo(200, 75)
        ->moveTo(50, 125)
        ->lineTo(200, 125)
        ->moveTo(50, 175)
        ->lineTo(200, 175)
        ->lineWidth = 30.0;

$context->lineCap = Context::LINE_CAP_ROUND;
$context->stroke();

$surface->writeToPNG("multi_segment_caps.png");
