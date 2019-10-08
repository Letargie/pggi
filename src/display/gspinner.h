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

#ifndef __GSPINNER_DEF__
#define __GSPINNER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"

zend_class_entry * gspinner_get_class_entry(void);

zend_object_handlers * gspinner_get_object_handlers(void);

/***********************************/
/* GSpinner class initialisation */
/***********************************/

void gspinner_init(int module_number);

#endif
