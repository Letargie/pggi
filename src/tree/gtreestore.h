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


#ifndef __GTREE_STORE_DEF__
#define __GTREE_STORE_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gtreeiter.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_remove, 0, 0, 1)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_insert, 0, 0, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, position)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_insert_next, 0, 0, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, sibling)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_insert_with_value, 0, 0, 3)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, position)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_pend, 0, 0, 0)
	ZEND_ARG_INFO(0, parent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_is_ancestor, 0, 0, 2)
	ZEND_ARG_INFO(0, iter)
	ZEND_ARG_INFO(0, descendant)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_depth, 0, 0, 1)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_store_get_column_type, 0, 0, 1)
	ZEND_ARG_INFO(0, column)
ZEND_END_ARG_INFO()


/************************************/
/* GTreeStore Intern Data Structure */
/************************************/

typedef struct{
	GtkTreeStore * intern;
} * gtree_store_ptr, gtree_store_t;

typedef struct{
	gtree_store_ptr tree_store_ptr;
	zend_object std;
} ze_gtree_store_object;

/*************************/
/* GTreeStore properties */
/*************************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gtree_store_get_class_entry(void);

zend_object_handlers * gtree_store_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtree_store_object *php_gtree_store_fetch_object(zend_object *obj) {
	return (ze_gtree_store_object *)((char*)(obj) - XtOffsetOf(ze_gtree_store_object, std));
}

static inline zend_object *php_gtree_store_reverse_object(ze_gtree_store_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gtree_store_ptr));
}

#define Z_GTREE_STORE_P(zv) php_gtree_store_fetch_object(Z_OBJ_P((zv)))

zend_object *gtree_store_object_new(zend_class_entry *class_type);

void gtree_store_dtor(gtree_store_ptr intern);

void gtree_store_object_free_storage(zend_object *object);
void gtree_store_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define GTREE_STORE_METHOD(name) \
PHP_METHOD(GTreeStore, name)

GTREE_STORE_METHOD(__construct     );
GTREE_STORE_METHOD(clear           );
//GTREE_STORE_METHOD(setValue);
//GTREE_STORE_METHOD(setValues);
//GTREE_STORE_METHOD(remove);
GTREE_STORE_METHOD(insert          );
GTREE_STORE_METHOD(insertBefore    );
GTREE_STORE_METHOD(insertAfter     );
GTREE_STORE_METHOD(insertWithValues);
GTREE_STORE_METHOD(prepend         );
GTREE_STORE_METHOD(append          );
GTREE_STORE_METHOD(isAncestor      );
GTREE_STORE_METHOD(depth           );
GTREE_STORE_METHOD(getNbColumns    );
GTREE_STORE_METHOD(getColumnType   );
GTREE_STORE_METHOD(getIterFirst    );
//GTREE_STORE_METHOD(foreach);

/************************************/
/* GTreeStore Class Initialization */
/************************************/

void gtree_store_init(int module_number);

/* __GTREE_STORE_DEF__ */

#endif
