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


#ifndef __GTOOL_ITEM_DEF__
#define __GTOOL_ITEM_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"

zend_class_entry * gtool_item_get_class_entry();

zend_object_handlers * gtool_item_get_object_handlers();

#define GTOOL_ITEM_HOMOGENEOUS  "homogeneous"
#define GTOOL_ITEM_EXPAND       "expand"

PHP_METHOD(GToolItem, __construct);

zval *gtool_item_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gtool_item_get_properties(zval *object);

PHP_WRITE_PROP_HANDLER_TYPE gtool_item_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gtool_item_init(int module_number);

#endif
