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


#ifndef __GBOX_DEF__
#define __GBOX_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gapplication.h"
#include "gcontainer.h"

zend_class_entry * gbox_get_class_entry();

zend_object_handlers * gbox_get_object_handlers();


ZEND_BEGIN_ARG_INFO_EX(arginfo_gbox_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, orientation)
	ZEND_ARG_INFO(0, spacing)
ZEND_END_ARG_INFO()

#define GBOX_HOMOGENEOUS		"homogeneous"
#define GBOX_SPACING			"spacing"
#define GBOX_BASELINE_POSITION	"baselinePosition"

PHP_METHOD(GBox, __construct);

zval *gbox_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gbox_get_properties(zval *object);

void gbox_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gbox_init(int module_number);

#endif
