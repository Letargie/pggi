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


#ifndef __GDRAWING_AREA_DEF__
#define __GDRAWING_AREA_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"

zend_class_entry * gdrawing_area_get_class_entry(void);

zend_object_handlers * gdrawing_area_get_object_handlers(void);

/***************/
/* PHP Methods */
/***************/

#define GDRAWING_AREA_METHOD(name) \
PHP_METHOD(GDrawingArea, name)

GDRAWING_AREA_METHOD(__construct);

/*************************************/
/* GDrawingArea class initialisation */
/*************************************/

void gdrawing_area_init(int module_number);

#endif
