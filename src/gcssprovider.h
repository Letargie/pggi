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


#ifndef __GCSS_PROVIDER_DEF__
#define __GCSS_PROVIDER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gcss_provider_load_from_path, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

/**************************************/
/* GCssProvider Intern Data Structure */
/**************************************/

typedef struct{
	GtkCssProvider * intern;
	zval signals;
} * gcss_provider_ptr, gcss_provider_t;

typedef struct{
	gcss_provider_ptr provider_ptr;
	zend_object std;
} ze_gcss_provider_object;

/***************************/
/* GCssProvider properties */
/***************************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gcss_provider_get_class_entry(void);

zend_object_handlers * gcss_provider_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gcss_provider_object *php_gcss_provider_fetch_object(zend_object *obj) {
	return (ze_gcss_provider_object *)((char*)(obj) - XtOffsetOf(ze_gcss_provider_object, std));
}

inline zend_object *php_gcss_provider_reverse_object(ze_gcss_provider_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gcss_provider_ptr));
}

#define Z_GCSS_PROVIDER_P(zv) php_gcss_provider_fetch_object(Z_OBJ_P((zv)))

gcss_provider_ptr gcss_provider_new();

zend_object *gcss_provider_object_new(zend_class_entry *class_type);

void gcss_provider_dtor(gcss_provider_ptr intern);

void gcss_provider_object_free_storage(zend_object *object);
void gcss_provider_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gcss_provider_func_changed(GtkCssProvider* context, gpointer data);

void gcss_provider_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GCSS_PROVIDER_METHOD(name) \
PHP_METHOD(GCssProvider, name)

GCSS_PROVIDER_METHOD(on          );
GCSS_PROVIDER_METHOD(__construct );
GCSS_PROVIDER_METHOD(loadFromPath);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gcss_provider_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gcss_provider_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gcss_provider_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/**************************************/
/* GCssProvider Class Initialization */
/**************************************/

void gcss_provider_init(int module_number);

/* __GCSS_PROVIDER_DEF__ */

#endif
