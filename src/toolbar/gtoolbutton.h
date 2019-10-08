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


#ifndef __GTOOL_BUTTON_DEF__
#define __GTOOL_BUTTON_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gtoolitem.h"


ZEND_BEGIN_ARG_INFO_EX(arginfo_gtool_button_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, icon)
	ZEND_ARG_INFO(0, label)
ZEND_END_ARG_INFO()


zend_class_entry * gtool_button_get_class_entry();

zend_object_handlers * gtool_button_get_object_handlers();

#define GTOOL_BUTTON_ICON_NAME     "iconName"
#define GTOOL_BUTTON_ICON_WIDGET   "iconWidget"
#define GTOOL_BUTTON_ICON_LABEL    "label"
#define GTOOL_BUTTON_LABEL_WIDGET  "labelWidget"
#define GTOOL_BUTTON_USE_UNDERLINE "useUnderline"

PHP_METHOD(GToolButton, __construct);

zval *gtool_button_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gtool_button_get_properties(zval *object);

PHP_WRITE_PROP_HANDLER_TYPE gtool_button_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gtool_button_init(int module_number);

#endif
