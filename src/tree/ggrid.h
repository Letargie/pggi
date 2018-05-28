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


#ifndef __GGRID_DEF__
#define __GGRID_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../containers/gbox.h"

zend_class_entry * ggrid_get_class_entry();

zend_object_handlers * ggrid_get_object_handlers();

#define GWIDGET_DATA_INDEX_GGRID 2

ZEND_BEGIN_ARG_INFO_EX(arginfo_ggrid_attach, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, widget, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, left, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, top, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ggrid_attach_next_to, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, widget, PGGI\\GWidget, 0)
	ZEND_ARG_OBJ_INFO(0, sibling, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, side, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 1)
ZEND_END_ARG_INFO()

//ZEND_BEGIN_ARG_INFO_EX(arginfo_ggrid_get_child_at, 0, 0, 0)
PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ggrid_get_child_at, 0, 0, PGGI\\GWidget, 1)
	ZEND_ARG_TYPE_INFO(0, left, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, top, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ggrid_insert_remove, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ggrid_insert_next_to, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, sibling, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, side, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define GGRID_BASELINE_ROW       "baselineRow"
#define GGRID_COLUMN_HOMOGENEOUS "columnHomogeneous"
#define GGRID_COLUMN_SPACING     "columnSpacing"
#define GGRID_ROW_HOMOGENEOUS    "rowHomogeneous"
#define GGRID_ROW_SPACING        "rowSpacing"

PHP_METHOD(GGrid, __construct);
PHP_METHOD(GGrid, attach);
PHP_METHOD(GGrid, attachNextTo);
PHP_METHOD(GGrid, getChildAt);
PHP_METHOD(GGrid, insertRow);
PHP_METHOD(GGrid, insertColumn);
PHP_METHOD(GGrid, removeRow);
PHP_METHOD(GGrid, removeColumn);
PHP_METHOD(GGrid, insertNextTo);

zval *ggrid_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *ggrid_get_properties(zval *object);

void ggrid_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void ggrid_init(int module_number);

#endif
