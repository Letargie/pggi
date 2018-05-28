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


#ifndef __GSTYLE_CONTEXT_DEF__
#define __GSTYLE_CONTEXT_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gcssprovider.h"
#include "../cairo/context.h"
#include "../gdk/rgba.h"

/***************************************/
/* GStyleContext Intern Data Structure */
/***************************************/

typedef struct{
	GtkStyleContext * intern;
	char own : 1;
	zval signals;
} * gstyle_context_ptr, gstyle_context_t;

typedef struct{
	gstyle_context_ptr context_ptr;
	zend_object std;
} ze_gstyle_context_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_gstyle_context_add_provider, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, provider, PGGI\\GCssProvider, 0)
	ZEND_ARG_TYPE_INFO(0, priority, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gstyle_context_render_background, 0, 0, 5)
	ZEND_ARG_OBJ_INFO(0, context, PGGI\\Cairo\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_gstyle_context_get_color, 0, 1, rgba_get_class_entry(), 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/****************************/
/* GStyleContext properties */
/****************************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gstyle_context_get_class_entry(void);

zend_object_handlers * gstyle_context_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gstyle_context_object *php_gstyle_context_fetch_object(zend_object *obj) {
	return (ze_gstyle_context_object *)((char*)(obj) - XtOffsetOf(ze_gstyle_context_object, std));
}

static inline zend_object *php_gstyle_context_reverse_object(ze_gstyle_context_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gstyle_context_ptr));
}

#define Z_GSTYLE_CONTEXT_P(zv) php_gstyle_context_fetch_object(Z_OBJ_P((zv)))

gstyle_context_ptr gstyle_context_new();

zend_object *gstyle_context_object_new(zend_class_entry *class_type);

void gstyle_context_dtor(gstyle_context_ptr intern);

void gstyle_context_object_free_storage(zend_object *object);
void gstyle_context_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gstyle_context_func_changed(GtkStyleContext* context, gpointer data);

void gstyle_context_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GSTYLE_CONTEXT_METHOD(name) \
PHP_METHOD(GStyleContext, name)

GSTYLE_CONTEXT_METHOD(on              );
GSTYLE_CONTEXT_METHOD(__construct     );
GSTYLE_CONTEXT_METHOD(addProvider     );
GSTYLE_CONTEXT_METHOD(renderBackground);
GSTYLE_CONTEXT_METHOD(getState        );
GSTYLE_CONTEXT_METHOD(getColor        );

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gstyle_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gstyle_context_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gstyle_context_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/**************************************/
/* GStyleContext Class Initialization */
/**************************************/

void gstyle_context_init(int module_number);

/* __GSTYLE_CONTEXT_DEF__ */

#endif
