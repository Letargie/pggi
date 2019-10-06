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


#ifndef __GTREE_SELECTION_DEF__
#define __GTREE_SELECTION_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gtreeiter.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_selection_iter, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, iter, PGGI\\GTreeIter, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_gtree_selection_iter_is_selected, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, iter, PGGI\\GTreeIter, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_gtree_selection_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_gtree_selection_get_selected, 0, 0, "PGGI\\GTreeIter", 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_selection_foreach, 0, 0, 1)
	ZEND_ARG_CALLABLE_INFO(0, callback, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

/****************************************/
/* GTreeSelection Intern Data Structure */
/****************************************/

typedef struct{
	GtkTreeSelection * intern;
	zval signals;
} * gtree_selection_ptr, gtree_selection_t;

typedef struct{
	gtree_selection_ptr tree_selection_ptr;
	zend_object std;
} ze_gtree_selection_object;

/*****************************/
/* GTreeSelection properties */
/*****************************/

#define GTREE_SELECTION_MODE "mode"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gtree_selection_get_class_entry(void);

zend_object_handlers * gtree_selection_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtree_selection_object *php_gtree_selection_fetch_object(zend_object *obj) {
	return (ze_gtree_selection_object *)((char*)(obj) - XtOffsetOf(ze_gtree_selection_object, std));
}

static inline zend_object *php_gtree_selection_reverse_object(ze_gtree_selection_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gtree_selection_ptr));
}

#define Z_GTREE_SELECTION_P(zv) php_gtree_selection_fetch_object(Z_OBJ_P((zv)))

gtree_selection_ptr gtree_selection_new();

zend_object *gtree_selection_object_new(zend_class_entry *class_type);

void gtree_selection_dtor(gtree_selection_ptr intern);

void gtree_selection_object_free_storage(zend_object *object);
void gtree_selection_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gtree_selection_func_changed(GtkTreeSelection* tree, gpointer data);

void gtree_selection_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GTREE_SELECTION_METHOD(name) \
PHP_METHOD(GTreeSelection, name)

GTREE_SELECTION_METHOD(on               );
GTREE_SELECTION_METHOD(__construct      );
GTREE_SELECTION_METHOD(getSelected      );
GTREE_SELECTION_METHOD(forEach          );
GTREE_SELECTION_METHOD(countSelectedRows);
GTREE_SELECTION_METHOD(selectIter       );
GTREE_SELECTION_METHOD(unselectIter     );
GTREE_SELECTION_METHOD(iterIsSelected   );
GTREE_SELECTION_METHOD(selectAll        );
GTREE_SELECTION_METHOD(unselectAll      );

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtree_selection_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gtree_selection_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gtree_selection_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/***************************************/
/* GTreeSelection Class Initialization */
/***************************************/

void gtree_selection_init(int module_number);

/* __GTREE_SELECTION_DEF__ */

#endif
