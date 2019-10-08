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


#ifndef __GPIXBUF_DEF__
#define __GPIXBUF_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_pixbuf_construct, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, has_alpha      , _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, bits_per_sample, IS_LONG , 0)
	ZEND_ARG_TYPE_INFO(0, width          , IS_LONG , 0)
	ZEND_ARG_TYPE_INFO(0, height         , IS_LONG , 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_pixbuf_create_from_file, 0, 1, "PGGI\\GDK\\Pixbuf", 0)
	ZEND_ARG_TYPE_INFO(0, filename      , IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, width         , IS_LONG  , 0)
	ZEND_ARG_TYPE_INFO(0, height        , IS_LONG  , 0)
	ZEND_ARG_TYPE_INFO(0, preserve_ratio, _IS_BOOL , 0)
ZEND_END_ARG_INFO()

/*********************************/
/* GPixbuf Intern Data Structure */
/*********************************/

typedef struct{
	GdkPixbuf * intern;
	int to_destroy;
} * gpixbuf_ptr, gpixbuf_t;

typedef struct{
	gpixbuf_ptr pixbuf_ptr;
	zend_object std;
} ze_gpixbuf_object;

/**********************/
/* GPixBuf properties */
/**********************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gpixbuf_get_class_entry(void);

zend_object_handlers * gpixbuf_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gpixbuf_object *php_gpixbuf_fetch_object(zend_object *obj) {
	return (ze_gpixbuf_object *)((char*)(obj) - XtOffsetOf(ze_gpixbuf_object, std));
}

static inline zend_object *php_gpixbuf_reverse_object(ze_gpixbuf_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gpixbuf_ptr));
}

#define Z_GPIXBUF_P(zv) php_gpixbuf_fetch_object(Z_OBJ_P((zv)))

gpixbuf_ptr gpixbuf_new();

zend_object *gpixbuf_object_new(zend_class_entry *class_type);

void gpixbuf_dtor(gpixbuf_ptr intern);

void gpixbuf_object_free_storage(zend_object *object);
void gpixbuf_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define GPIXBUF_METHOD(name) \
PHP_METHOD(GPixbuf, name)

GPIXBUF_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gpixbuf_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gpixbuf_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gpixbuf_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/********************************/
/* GPixbuf Class Initialization */
/********************************/

void gpixbuf_init(int module_number);

/* __GPIXBUF_DEF__ */

#endif
