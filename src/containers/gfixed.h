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


#ifndef __GFIXED_DEF__
#define __GFIXED_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"

zend_class_entry * gfixed_get_class_entry();

zend_object_handlers * gfixed_get_object_handlers();

ZEND_BEGIN_ARG_INFO_EX(arginfo_gfixed_manipulate_widget_position, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, object, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(GFixed, __construct);
PHP_METHOD(GFixed, put);
PHP_METHOD(GFixed, move);

void gfixed_init(int module_number);

#endif
