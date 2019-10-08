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


#ifndef __GTOOLBAR_DEF__
#define __GTOOLBAR_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"
#include "gtoolitem.h"

zend_class_entry * gtoolbar_get_class_entry();

zend_object_handlers * gtoolbar_get_object_handlers();


ZEND_BEGIN_ARG_INFO_EX(arginfo_gtoolbar_construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, orientation, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, spacing, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtoolbar_insert, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, object, PGGI\\GToolItem, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 1)
ZEND_END_ARG_INFO()

#define GTOOLBAR_SHOW_ARROW "showArrow"
#define GTOOLBAR_STYLE      "style"

PHP_METHOD(GToolbar, __construct);
PHP_METHOD(GToolbar, packStart);
PHP_METHOD(GToolbar, packEnd);

zval *gtoolbar_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gtoolbar_get_properties(zval *object);

PHP_WRITE_PROP_HANDLER_TYPE gtoolbar_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gtoolbar_init(int module_number);

#endif
