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


#ifndef __GEVENT_DEF__
#define __GEVENT_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

/********************************/
/* GEvent Intern Data Structure */
/********************************/

typedef struct{
} * gevent_ptr, gevent_t;

typedef struct{
	gevent_ptr event_ptr;
	zend_object std;
} ze_gevent_object;

/*********************/
/* GEvent properties */
/*********************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gevent_get_class_entry(void);

zend_object_handlers * gevent_get_object_handlers();

zend_object * gevent_ctor(zend_class_entry *ce, GdkEvent * event);

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gevent_object *php_gevent_fetch_object(zend_object *obj) {
	return (ze_gevent_object *)((char*)(obj) - XtOffsetOf(ze_gevent_object, std));
}

static inline zend_object *php_gevent_reverse_object(ze_gevent_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gevent_ptr));
}

#define Z_GEVENT_P(zv) php_gevent_fetch_object(Z_OBJ_P((zv)))

zend_object *gevent_object_new(zend_class_entry *class_type);

void gevent_dtor(gevent_ptr intern);

void gevent_object_free_storage(zend_object *object);
void gevent_free_resource(zend_resource *rsrc);

gevent_ptr gevent_new();

/***************/
/* PHP Methods */
/***************/

#define GEVENT_METHOD(name) \
PHP_METHOD(GEvent, name)

GEVENT_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gevent_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gevent_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gevent_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/*******************************/
/* GEvent Class Initialization */
/*******************************/

void gevent_init(int module_number);

/* __GEVENT_DEF__ */

#endif
