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

#ifndef PHP_PANGO_LAYOUT_DEF
#define PHP_PANGO_LAYOUT_DEF

#include <pango/pango.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "context.h"
#include "../cairo/context.h"

/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	PangoLayout * intern;
	int to_destroy : 1;
} * pp_layout_ptr, pp_layout_t;

typedef struct{
	pp_layout_ptr layout_ptr;
	zend_object std;
} ze_layout_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_layout_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, layout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_layout_get_size, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(1, value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, value, IS_LONG, 0)
ZEND_END_ARG_INFO()


/**************/
/* properties */
/**************/

#define LAYOUT_WIDTH  "width"
#define LAYOUT_HEIGHT "height"
#define LAYOUT_WRAP   "wrap"
#define LAYOUT_TEXT   "text"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pp_layout_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pp_layout_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

pp_layout_ptr pp_layout_new(void);

static inline ze_layout_object *php_layout_fetch_object(zend_object *obj) {
	return (ze_layout_object *)((char*)(obj) - XtOffsetOf(ze_layout_object, std));
}

static inline zend_object *php_layout_reverse_object(ze_layout_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pp_layout_ptr));
}

#define Z_LAYOUT_P(zv) php_layout_fetch_object(Z_OBJ_P((zv)))

zend_object *pp_layout_object_new(zend_class_entry *class_type);

void pp_layout_dtor(pp_layout_ptr intern);

void pp_layout_object_free_storage(zend_object *object);
void pp_layout_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define LAYOUT_METHOD(name) \
PHP_METHOD(Layout, name)

/* layout Functions */

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pp_layout_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pp_layout_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE pp_layout_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pp_layout_init(int module_number);





#endif
