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


#ifndef __GMENU_DEF__
#define __GMENU_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gmenushell.h"

/***************/
/* PHP Methods */
/***************/

#define GMENU_METHOD(name) \
PHP_METHOD(GMenu, name)

GMENU_METHOD(__construct);

/******************************/
/* GMenu class initialisation */
/******************************/

void gmenu_init(int module_number);

#endif
