<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);


$context->lineWidth = 40.96;
$context->moveTo(76.8, 84.48)
        ->relLineTo(51.2, -51.2)
        ->relLineTo(51.2, 51.2)
        ->lineJoin = Context::LINE_JOIN_MITER; // default one
$context->stroke()
        ->moveTo(76.8, 161.28)
        ->relLineTo(51.2, -51.2)
        ->relLineTo(51.2, 51.2)
        ->lineJoin = Context::LINE_JOIN_BEVEL;
$context->stroke()
        ->moveTo(76.8, 238.08)
        ->relLineTo(51.2, -51.2)
        ->relLineTo(51.2, 51.2)
        ->lineJoin = Context::LINE_JOIN_ROUND;
$context->stroke();

$surface->writeToPNG("lineJoin.png");
