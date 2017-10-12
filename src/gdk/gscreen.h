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


#ifndef __GSCREEN_DEF__
#define __GSCREEN_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../theming/gcssprovider.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gscreen_add_provider, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, provider, PGGI\\GCssProvider, 0)
	ZEND_ARG_TYPE_INFO(0, priority, IS_LONG, 0)
ZEND_END_ARG_INFO()

/*********************************/
/* GScreen Intern Data Structure */
/*********************************/

typedef struct{
	GdkScreen * intern;
	zval signals;
} * gscreen_ptr, gscreen_t;

typedef struct{
	gscreen_ptr screen_ptr;
	zend_object std;
} ze_gscreen_object;

/**********************/
/* GScreen properties */
/**********************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gscreen_get_class_entry(void);

zend_object_handlers * gscreen_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gscreen_object *php_gscreen_fetch_object(zend_object *obj) {
	return (ze_gscreen_object *)((char*)(obj) - XtOffsetOf(ze_gscreen_object, std));
}

inline zend_object *php_gscreen_reverse_object(ze_gscreen_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gscreen_ptr));
}

#define Z_GSCREEN_P(zv) php_gscreen_fetch_object(Z_OBJ_P((zv)))

gscreen_ptr gscreen_new();

zend_object *gscreen_object_new(zend_class_entry *class_type);

void gscreen_dtor(gscreen_ptr intern);

void gscreen_object_free_storage(zend_object *object);
void gscreen_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gscreen_func_changed(GdkScreen* context, gpointer data);

void gscreen_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GSCREEN_METHOD(name) \
PHP_METHOD(GScreen, name)

GSCREEN_METHOD(on          );
GSCREEN_METHOD(__construct );

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gscreen_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gscreen_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gscreen_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/********************************/
/* GScreen Class Initialization */
/********************************/

void gscreen_init(int module_number);

/* __GSCREEN_DEF__ */

#endif
