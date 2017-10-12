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


#ifndef __GMENUBAR_DEF__
#define __GMENUBAR_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gmenushell.h"

/***************/
/* PHP Methods */
/***************/

#define GMENUBAR_METHOD(name) \
PHP_METHOD(GMenubar, name)

GMENUBAR_METHOD(__construct);

/******************************/
/* GMenubar class initialisation */
/******************************/

void gmenubar_init(int module_number);

#endif
