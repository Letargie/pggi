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

#ifndef __GCONTAINER_DEF__
#define __GCONTAINER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gwidget.h"

zend_class_entry * gcontainer_get_class_entry(void);

zend_object_handlers * gcontainer_get_object_handlers(void);

void gcontainer_add_data(gwidget_ptr intern, zval * data);


/***********************/
/* GContainer Arginfos */
/***********************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gcontainer_add, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()


/*************************/
/* GContainer Properties */
/*************************/

#define GCONTAINER_BORDER_WIDTH "borderWidth"

/*****************/
/* PHP FUNCTIONS */
/*****************/

PHP_METHOD(GContainer, add);
PHP_METHOD(GContainer, __construct);
PHP_METHOD(GContainer, on);

void gcontainer_func_add(GtkWidget* container, gpointer data);

void gcontainer_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param);

/*****************************/
/* Object handling functions */
/*****************************/

zval *gcontainer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gcontainer_get_properties(zval *object);

void gcontainer_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/***********************************/
/* GContainer class initialisation */
/***********************************/

void gcontainer_init(int module_number);

#endif
