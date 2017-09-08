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


#ifndef __GTREE_ITER_DEF__
#define __GTREE_ITER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"

/*
ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_iter_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, parent)
ZEND_END_ARG_INFO()
*/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_iter_get_value, 0, 0, 1)
	ZEND_ARG_INFO(0, column)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_iter_set_value, 0, 0, 2)
	ZEND_ARG_INFO(0, column)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_iter_get_nth_child, 0, 0, 1)
	ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

/************************************/
/* GTreeIter Intern Data Structure */
/************************************/

typedef struct{
	GtkTreeIter * intern;
	GtkTreeModel * parent;
} * gtree_iter_ptr, gtree_iter_t;

typedef struct{
	gtree_iter_ptr tree_iter_ptr;
	zend_object std;
} ze_gtree_iter_object;

/*************************/
/* GTreeIter properties */
/*************************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gtree_iter_get_class_entry(void);

zend_object_handlers * gtree_iter_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtree_iter_object *php_gtree_iter_fetch_object(zend_object *obj) {
	return (ze_gtree_iter_object *)((char*)(obj) - XtOffsetOf(ze_gtree_iter_object, std));
}

inline zend_object *php_gtree_iter_reverse_object(ze_gtree_iter_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gtree_iter_ptr));
}

#define Z_GTREE_ITER_P(zv) php_gtree_iter_fetch_object(Z_OBJ_P((zv)))

zend_object *gtree_iter_object_new(zend_class_entry *class_type);

void gtree_iter_dtor(gtree_iter_ptr intern);

void gtree_iter_object_free_storage(zend_object *object);
void gtree_iter_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define GTREE_ITER_METHOD(name) \
PHP_METHOD(GTreeIter, name)

GTREE_ITER_METHOD(__construct);
GTREE_ITER_METHOD(getValue);
GTREE_ITER_METHOD(setValue);
GTREE_ITER_METHOD(next);
GTREE_ITER_METHOD(previous);
GTREE_ITER_METHOD(getFirstChild);
GTREE_ITER_METHOD(hasChild);
GTREE_ITER_METHOD(getNbChildren);
GTREE_ITER_METHOD(getNthChild);
GTREE_ITER_METHOD(getParent);
GTREE_ITER_METHOD(remove);

/************************************/
/* GTreeIter Class Initialization */
/************************************/

void gtree_iter_init(int module_number);

/* __GTREE_ITER_DEF__ */

#endif
