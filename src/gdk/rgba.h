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


#ifndef __RGBA_DEF__
#define __RGBA_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

typedef struct{
	GdkRGBA * color;
	int to_destroy : 1;
} * rgba_ptr, rgba_t;

typedef struct{
	rgba_ptr ptr;
	zend_object std;
} ze_rgba_object;

#define RGBA_RED   "red"
#define RGBA_GREEN "green"
#define RGBA_BLUE  "blue"
#define RGBA_ALPHA "alpha"

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rgba_equals, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, widget, PGGI\\RGBA, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_rgba_copy, 0, 0, rgba_get_class_entry(), 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rgba_construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * rgba_get_class_entry(void);

zend_object_handlers * rgba_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_rgba_object *php_rgba_fetch_object(zend_object *obj) {
	return (ze_rgba_object *)((char*)(obj) - XtOffsetOf(ze_rgba_object, std));
}

inline zend_object *php_rgba_reverse_object(ze_rgba_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(rgba_ptr));
}

#define Z_RGBA_P(zv) php_rgba_fetch_object(Z_OBJ_P((zv)))

zend_object *rgba_object_new(zend_class_entry *class_type);

void rgba_dtor(rgba_ptr intern);

void rgba_object_free_storage(zend_object *object);
void rgba_free_resource(zend_resource *rsrc);

rgba_ptr rgba_new();

/***************/
/* PHP Methods */
/***************/

#define RGBA_METHOD(name) \
PHP_METHOD(RGBA, name)

RGBA_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *rgba_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *rgba_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void rgba_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void rgba_init(int module_number);


#endif
