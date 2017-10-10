/*
  +-----------------------------------------------------------+
  | Copyright (c) 2017 Collet Valentin                        |
  +-----------------------------------------------------------+
  | This source file is subject to version the BDS license,   |
  | that is bundled with this package in the file LICENSE     |
  +-----------------------------------------------------------+
  | Author: Collet Valentin <valentin@famillecollet.com>      |
  +-----------------------------------------------------------+
*/

#ifndef PHP_CAIRO_DEF
#define PHP_CAIRO_DEF

#define PHP_CAIRO_VERSION "0.1.0"

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "exception.h"
#include "context.h"
#include "surface.h"
#include "imagesurface.h"

void cairo_minit(int module_number);

#define pphext_cairo_ptr &cairo_module_entry

#endif
