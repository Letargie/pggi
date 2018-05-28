<?php

namespace PGGI\Cairo;

chdir(__DIR__);

$surface = new ImageSurface(ImageSurface::FORMAT_ARGB32, 300, 300);
$context = new Context($surface);

$context->lineWidth = 6.0;
$context->rectangle(12, 12, 232, 70)
        ->newSubPath()
        ->arc(64, 64, 40, 0, 2*pi())
        ->newSubPath()
        ->arcNegative(192, 64, 40, 0, -2*pi())
        ->fillRule = Context::FILL_RULE_EVEN_ODD;
$context->setSource(0, 0.7, 0)
        ->fillPreserve()
        ->setSource(0, 0, 0)
        ->stroke()
        ->translate(0, 128)
        ->rectangle(12, 12, 232, 70)
        ->newSubPath()
        ->arc(64, 64, 40, 0, 2*pi())
        ->newSubPath()
        ->arcNegative(192, 64, 40, 0, -2*pi())
        ->fillRule = Context::FILL_RULE_WINDING;
$context->setSource(0, 0, 0.9)
        ->fillPreserve()
        ->setSource(0, 0, 0)
        ->stroke();

$surface->writeToPNG("fill_style.png");

