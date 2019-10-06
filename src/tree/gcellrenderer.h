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


#ifndef __GCELL_RENDERER_DEF__
#define __GCELL_RENDERER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

/***************************************/
/* GCellRenderer Intern Data Structure */
/***************************************/

typedef struct{
	GtkCellRenderer * intern;
	zval signals;
} * gcell_renderer_ptr, gcell_renderer_t;

typedef struct{
	gcell_renderer_ptr cell_renderer_ptr;
	zend_object std;
} ze_gcell_renderer_object;

/****************************/
/* GCellRenderer properties */
/****************************/

#define GCELL_RENDERER_VISIBLE  "visible"
#define GCELL_RENDERER_SENSIBLE "sensible"
#define GCELL_RENDERER_X_ALIGN  "xAlign"
#define GCELL_RENDERER_Y_ALIGN  "yAlign"
#define GCELL_RENDERER_X_PAD    "xPad"
#define GCELL_RENDERER_Y_PAD    "yPad"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gcell_renderer_get_class_entry(void);

zend_object_handlers * gcell_renderer_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gcell_renderer_object *php_gcell_renderer_fetch_object(zend_object *obj) {
	return (ze_gcell_renderer_object *)((char*)(obj) - XtOffsetOf(ze_gcell_renderer_object, std));
}

static inline zend_object *php_gcell_renderer_reverse_object(ze_gcell_renderer_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gcell_renderer_ptr));
}

#define Z_GCELL_RENDERER_P(zv) php_gcell_renderer_fetch_object(Z_OBJ_P((zv)))

gcell_renderer_ptr gcell_renderer_new();

zend_object *gcell_renderer_object_new(zend_class_entry *class_type);

void gcell_renderer_dtor(gcell_renderer_ptr intern);

void gcell_renderer_object_free_storage(zend_object *object);
void gcell_renderer_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gcell_renderer_func_changed(GtkCellRenderer* buffer, gpointer data);

void gcell_renderer_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GCELL_RENDERER_METHOD(name) \
PHP_METHOD(GCellRenderer, name)

GCELL_RENDERER_METHOD(on          );
GCELL_RENDERER_METHOD(getFixedSize); // int int out
GCELL_RENDERER_METHOD(setFixedSize); // int int
GCELL_RENDERER_METHOD(getAlignment); // int int out
GCELL_RENDERER_METHOD(setAlignment); // int int
GCELL_RENDERER_METHOD(getPadding  ); // int int out
GCELL_RENDERER_METHOD(setPadding  ); // int int

GCELL_RENDERER_METHOD(activable);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
//zval *gcell_renderer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
//HashTable *gcell_renderer_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
//PHP_WRITE_PROP_HANDLER_TYPE gcell_renderer_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************************/
/* GCellRenderer Class Initialization */
/************************************/

void gcell_renderer_init(int module_number);

/* __GCELL_RENDERER_DEF__ */

#endif
