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


#ifndef __GWINDOW_DEF__
#define __GWINDOW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../cairo/context.h"

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_gdk_window_create_similar_surface, 0, 0, context_get_class_entry(), 0)
	ZEND_ARG_TYPE_INFO(0, content, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width  , IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height , IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gdk_window_draw_frame, 0, 0, 1)
	ZEND_ARG_CALLABLE_INFO(0, callback, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

/*********************************/
/* GWindow Intern Data Structure */
/*********************************/

typedef struct{
	GdkWindow * intern;
} * gdk_gwindow_ptr, gdk_gwindow_t;

typedef struct{
	gdk_gwindow_ptr window_ptr;
	zend_object std;
} ze_gdk_gwindow_object;

/**********************/
/* GWindow properties */
/**********************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gdk_gwindow_get_class_entry(void);

zend_object_handlers * gdk_gwindow_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gdk_gwindow_object *php_gdk_gwindow_fetch_object(zend_object *obj) {
	return (ze_gdk_gwindow_object *)((char*)(obj) - XtOffsetOf(ze_gdk_gwindow_object, std));
}

static inline zend_object *php_gdk_gwindow_reverse_object(ze_gdk_gwindow_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gdk_gwindow_ptr));
}

#define Z_GDK_GWINDOW_P(zv) php_gdk_gwindow_fetch_object(Z_OBJ_P((zv)))

gdk_gwindow_ptr gdk_gwindow_new();

zend_object *gdk_gwindow_object_new(zend_class_entry *class_type);

void gdk_gwindow_dtor(gdk_gwindow_ptr intern);

void gdk_gwindow_object_free_storage(zend_object *object);
void gdk_gwindow_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define GDK_WINDOW_METHOD(name) \
	PREFIX_METHOD(GDK, Window, name)

#define GDK_WINDOW_ME(name, arg_info, flags) \
	PREFIX_ME(GDK, Window, name, arg_info, flags)

GDK_WINDOW_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gdk_gwindow_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gdk_gwindow_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gdk_gwindow_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/********************************/
/* GWindow Class Initialization */
/********************************/

void gdk_gwindow_init(int module_number);

/* __GWINDOW_DEF__ */

#endif
